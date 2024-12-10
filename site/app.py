from flask import Flask, request, jsonify, send_file, render_template
import subprocess
import os
import logging
import uuid

app = Flask(__name__)

# Путь к основной директории, где будут создаваться уникальные папки
BASE_UPLOAD_FOLDER = "uploads"
os.makedirs(BASE_UPLOAD_FOLDER, exist_ok=True)  # Создать основную директорию, если её нет

# Настройка логирования
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')

# Путь к скомпилированному C++ исполнимому файлу
encoder_path = os.path.join(os.getcwd(), "encoder.exe")

unique_folder_path = ''
file_name = ''

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/encrypt', methods=['POST'])
def encrypt_file():
    global unique_folder_path
    global file_name

    # Получаем данные из запроса
    input_file = request.files['inputFile']
    modulus = int(request.form['modulus'])
    method_choice = int(request.form['methodChoice'])

     # Логирование начала работы
    logging.debug(f"Запуск шифрования файла: {input_file.filename}, Модуль: {modulus}, Метод: {method_choice}")

    # Генерация уникального идентификатора для директории
    unique_id = str(uuid.uuid4())
    unique_folder_path = os.path.join(BASE_UPLOAD_FOLDER, unique_id)
    os.makedirs(unique_folder_path, exist_ok=True)  # Создать директорию

    # Получаем имя файла без пути
    file_name = os.path.basename(input_file.filename)

    # Полный путь для сохранения файла
    input_file_path = os.path.join(unique_folder_path, input_file.filename)
    input_file.save(input_file_path)

    # Формируем команду для шифрования
    command = [encoder_path, 'encrypt', input_file_path, str(modulus), str(method_choice)]

    # Запускаем C++ программу и получаем результат
    try:
        logging.debug(f"Запуск команды: {command}")
        subprocess.run(command, check=True)

        # Логирование успешного завершения
        logging.debug(f"Файл успешно зашифрован: {input_file.filename}")
        
        # Путь к зашифрованному файлу
        encrypted_file_path = os.path.join(unique_folder_path, "encrypted_" + file_name)
        
        # Возвращаем файл для скачивания
        return send_file(encrypted_file_path, as_attachment=True)

    except subprocess.CalledProcessError as e:
        logging.error(f"Ошибка при шифровании: {str(e)}")
        return jsonify({"error": f"Ошибка при шифровании: {str(e)}"}), 500
    
@app.route('/download-gamma', methods=['GET'])
def download_gamma():
    global unique_folder_path
    global file_name

    # Формируем путь к файлу гаммы
    gamma_file_path = os.path.join(unique_folder_path, "gamma.txt" )
    logging.debug(f"Ищем файл гаммы: {gamma_file_path}")

    # Логирование запроса на скачивание
    logging.debug("Запрос на скачивание гаммы")

    if os.path.exists(gamma_file_path):
        # Логирование успешного нахождения файла гаммы
        logging.debug(f"Файл гаммы найден: {gamma_file_path}")
        return send_file(gamma_file_path, as_attachment=True)
    else:
        # Логирование ошибки, если файл не найден
        logging.error(f"Файл гаммы не найден: {gamma_file_path}")
        return jsonify({"error": "Файл гаммы не найден"}), 404


@app.route('/decrypt', methods=['POST'])
def decrypt_file():
    # Получаем данные из запроса
    input_file = request.files['inputFileDecrypt']
    gamma_file = request.files['gammaFile']
    length_byte = int(request.form['lengthByte'])
    checksum_byte = int(request.form['checksumByte'])

    # Логирование начала работы
    logging.debug(f"Запуск дешифрования файла: {input_file.filename}, гамма файл: {gamma_file.filename}, длина байта: {length_byte}, контрольная сумма: {checksum_byte}")

    # Генерация уникального идентификатора для директории
    unique_id = str(uuid.uuid4())
    unique_folder_path = os.path.join(BASE_UPLOAD_FOLDER, unique_id)
    os.makedirs(unique_folder_path, exist_ok=True)  # Создать директорию

    # Получаем имя файла без пути
    file_name = os.path.basename(input_file.filename)

    # Полный путь для сохранения файла
    input_file_path = os.path.join(unique_folder_path, input_file.filename)
    input_file.save(input_file_path)
    gamma_file_path = os.path.join(unique_folder_path, gamma_file.filename)
    gamma_file.save(gamma_file_path)

    # Формируем команду для дешифрования
    command = [
        encoder_path,
        'decrypt',
        input_file_path,
        gamma_file_path,
        str(length_byte),
        str(checksum_byte)
    ]

    # Запускаем C++ программу и получаем результат
    try:
        logging.debug(f"Запуск команды: {command}")
        subprocess.run(command, check=True)

        # Логирование успешного завершения
        logging.debug(f"Файл успешно расшифрован: {input_file.filename}")
        
        # Путь к расшифрованному файлу
        decrypted_file_path = os.path.join(unique_folder_path, "decrypted_" + file_name )
        
        # Возвращаем файл для скачивания
        return send_file(decrypted_file_path, as_attachment=True)

    except subprocess.CalledProcessError as e:
        logging.error(f"Ошибка при дешифровании: {str(e)}")
        return jsonify({"error": f"Ошибка при дешифровании: {str(e)}"}), 500


if __name__ == '__main__':
    app.run(debug=True)