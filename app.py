from flask import Flask, request, send_file, render_template_string, render_template
import subprocess
import os

app = Flask(__name__)

# Главная страница с интерфейсом
@app.route('/')
def index():
    return render_template('index.html')  # Ожидается, что index.html будет в папке "templates"

# Маршрут для обработки шифрования
@app.route('/encrypt', methods=['POST'])
def encrypt_file():
    file = request.files['file']
    method = request.form['method']
    modulus = request.form['modulus']
    
    # Сохраняем загруженный файл
    input_filename = 'input.txt'
    file.save(input_filename)

    # Запускаем encryptor.exe с параметрами
    subprocess.run(['./encryptor.exe', input_filename, modulus, method])

    # Отправляем зашифрованный файл обратно
    return send_file(input_filename, as_attachment=True, download_name="encrypted_file.txt")

@app.route('/decrypt', methods=['POST'])
def decrypt_file():
    # Получаем данные от формы
    file = request.files['decryptFile']
    gamma = request.files['gammaFile']
    length_byte = request.form['lengthByte']
    checksum_byte = request.form['checksumByte']

    # Сохраняем загруженные файлы
    decrypt_filename = 'decrypt_input.txt'
    gamma_filename = 'gamma_file.txt'
    output_filename = 'decrypted_output.txt'
    file.save(decrypt_filename)
    gamma.save(gamma_filename)

    # Преобразуем радиокнопки в 'y' или 'n'
    length_arg = 'y' if length_byte == 'yes' else 'n'
    checksum_arg = 'y' if checksum_byte == 'yes' else 'n'

    # Запуск decryptor.exe
    result = subprocess.run(['./decryptor.exe', decrypt_filename, gamma_filename, length_arg, checksum_arg])

    # Проверка на ошибки выполнения
    if result.returncode != 0:
        print("Ошибка выполнения decryptor.exe")
        return "Ошибка при дешифровке файла", 500

    # Отправка расшифрованного файла
    return send_file(output_filename, as_attachment=True, download_name="decrypted_file.txt")


    
# Маршрут для скачивания файла гаммы
@app.route('/download-gamma', methods=['GET'])
def download_gamma():
    return send_file('gamma.txt', as_attachment=True)

if __name__ == '__main__':
    app.run(debug=True)
