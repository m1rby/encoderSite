import os
import uuid
from flask import Flask, request, send_file, render_template
import subprocess

app = Flask(__name__)

# Главная страница с интерфейсом
@app.route('/')
def index():
    return render_template('index.html')  # Ожидается, что index.html будет в папке "templates"

@app.route('/encrypt', methods=['POST'])
def encrypt_file():
    file = request.files['file']
    method = request.form['method']
    modulus = request.form['modulus']
    
    # Сохраняем файл с оригинальным именем
    input_filename = file.filename
    file.save(input_filename)

    # Запускаем encryptor.exe с параметрами
    subprocess.run(['./encryptor.exe', input_filename, modulus, method])

    # Возвращаем зашифрованный файл с оригинальным именем
    return send_file(input_filename)

# Маршрут для обработки расшифровки
@app.route('/decrypt', methods=['POST'])
def decrypt_file():
    # Получаем данные от формы
    file = request.files['decryptFile']
    gamma = request.files['gammaFile']
    length_byte = request.form['lengthByte']
    checksum_byte = request.form['checksumByte']

    # Сохраняем файл с оригинальным именем
    decrypt_filename = file.filename
    file.save(decrypt_filename)
    
    gamma_filename = gamma.filename
    gamma.save(gamma_filename)

    # Преобразуем радиокнопки в 'y' или 'n'
    length_arg = 'y' if length_byte == 'yes' else 'n'
    checksum_arg = 'y' if checksum_byte == 'yes' else 'n'

    # Запуск decryptor.exe
    subprocess.run(['./decryptor.exe', decrypt_filename, gamma_filename, length_arg, checksum_arg], check=True)



    # Возвращаем расшифрованный файл
    return send_file(decrypt_filename, as_attachment=True)




# Маршрут для скачивания файла гаммы
@app.route('/download-gamma', methods=['GET'])
def download_gamma():
    return send_file('gamma.txt', as_attachment=True)


if __name__ == '__main__':
    app.run(debug=True)