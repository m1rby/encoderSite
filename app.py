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

# Маршрут для скачивания файла гаммы
@app.route('/download-gamma', methods=['GET'])
def download_gamma():
    return send_file('gamma.txt', as_attachment=True)

if __name__ == '__main__':
    app.run(debug=True)
