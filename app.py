from flask import Flask, request, send_file, render_template
import subprocess
import os
import uuid

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
    
    # Уникальное имя файла для предотвращения коллизий
    input_filename = f"{uuid.uuid4()}_{file.filename}"
    file.save(input_filename)

    # Запускаем encryptor.exe с параметрами
    subprocess.run(['./encryptor.exe', input_filename, modulus, method])

    # Отправляем зашифрованный файл обратно
    encrypted_filename = f"encrypted_{file.filename}"
    return send_file(input_filename, as_attachment=True, download_name=encrypted_filename)

import os

@app.route('/decrypt', methods=['POST'])
def decrypt_file():
    # Получаем данные от формы
    file = request.files['decryptFile']
    gamma = request.files['gammaFile']
    length_byte = request.form['lengthByte']
    checksum_byte = request.form['checksumByte']

    # Уникальные имена файлов для предотвращения коллизий
    decrypt_filename = f"{uuid.uuid4()}_{file.filename}"
    gamma_filename = f"{uuid.uuid4()}_gamma_file"
    file.save(decrypt_filename)
    gamma.save(gamma_filename)

    # Преобразуем радиокнопки в 'y' или 'n'
    length_arg = 'y' if length_byte == 'yes' else 'n'
    checksum_arg = 'y' if checksum_byte == 'yes' else 'n'

    # Запуск decryptor.exe
    subprocess.run(['./decryptor.exe', decrypt_filename, gamma_filename, length_arg, checksum_arg], check=True)


    # Возврат зашифрованного файла
    return send_file(decrypt_filename, as_attachment=True, download_name=f"decrypted_{file.filename}")


# Маршрут для скачивания файла гаммы
@app.route('/download-gamma', methods=['GET'])
def download_gamma():
    return send_file('gamma.txt', as_attachment=True)

if __name__ == '__main__':
    app.run(debug=True)
