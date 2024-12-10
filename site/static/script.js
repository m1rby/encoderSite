let isEncrypted = false;
let uploadedEncryptFileName = '';
let uploadedDecryptFileName = '';

document.getElementById("encryptForm").addEventListener("submit", function (event) {
    event.preventDefault();

    var formData = new FormData(this);
    var inputFile = document.getElementById('inputFile'); // Элемент формы для загрузки файла

    uploadedEncryptFileName = inputFile.files[0].name; // Получаем имя первого загруженного файла
    const fileSize = inputFile.files[0];

    const maxFileSize = 100 * 1024 * 1024; // 5 MB

    if (fileSize.size > maxFileSize) {
        alert("Файл слишком большой. Максимальный размер: 100 MB.");
    }
    else {
        fetch('/encrypt', {
            method: 'POST',
            body: formData
        })
            .then(response => response.blob())
            .then(blob => {

                alert("Файл успешно зашифрован!");

                isEncrypted = true;
                const userConfirmed = confirm("Файл был зашифрован. Хотите скачать его?");

                if (userConfirmed) {
                    // Скачиваем зашифрованный файл
                    var link = document.createElement('a');
                    link.href = URL.createObjectURL(blob);
                    link.download = "encrypted_" + uploadedEncryptFileName;
                    link.click();

                    // Отображаем сообщение об успешном шифровании
                    document.getElementById("confirmationMessage").style.display = 'block';

                }

            })
            .catch(error => {
                console.error('Error:', error);
            });
    }
});

document.getElementById("downloadGammaBtn").addEventListener("click", function () {
    if (!isEncrypted) {
        // Если файл не был зашифрован, выводим предупреждение
        alert("Сначала загрузите и зашифруйте файл.");
        return;
    }

    fetch('/download-gamma')
        .then(response => {
            if (!response.ok) {
                throw new Error("Ошибка при загрузке гаммы");
            }
            return response.blob();
        })
        .then(blob => {
            var link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = "gamma.txt";
            link.click();
        })
        .catch(error => {
            console.error('Error:', error);
        });
});

document.getElementById("decryptForm").addEventListener("submit", function (event) {
    event.preventDefault();
    const formData = new FormData(this);
    const inputFile = document.getElementById('inputFileDecrypt'); // Элемент формы для загрузки файла

    uploadedEncryptFileName = inputFile.files[0].name; // Получаем имя первого загруженного файла
    const fileSize = inputFile.files[0];
    const maxFileSize = 100 * 1024 * 1024; // 5 MB

    if (fileSize.size > maxFileSize) {
        alert("Файл слишком большой. Максимальный размер: 100 MB.");
    }
    else {
        fetch('/decrypt', {
            method: 'POST',
            body: formData
        })
            .then(response => response.blob())
            .then(blob => {

                alert("Файл успешно расшифрован!");
                const userConfirmed = confirm("Файл был Расшифрован. Хотите скачать его?");

                if (userConfirmed) {
                    // Скачиваем зашифрованный файл
                    var link = document.createElement('a');
                    link.href = URL.createObjectURL(blob);
                    link.download = "decrypted_" + uploadedEncryptFileName;
                    link.click();
                }

                // Флаг для отслеживания завершения шифрования
                isEncrypted = true;
            })
            .catch(error => {
                console.error('Error:', error);
            });
    }
});

// Описания для каждого метода шифрования
const descriptions = {
    "1": "4.1.1 и 4.1.2 Блочный шифр гаммирования по модулю",
    "2": "4.2.1 и 4.2.4 Блочный шифр гаммирования с маркантом от секунд",
    "3": "4.2.2 и 4.2.5 Блочный шифр гаммирования с маркантом от числа месяца",
    "4": "4.2.3 и 4.2.6 Блочный шифр гаммирования с маркантом от дня недели",

    "5": "4.3.1 и 4.3.2 Блочный шифр гаммирования с полем длины открытого текста",
    "6": "4.4.1 и 4.4.2 Блочный шифр гаммирования с контрольной суммой",

    "7": "4.5.1 и 4.5.4 Блочный шифр гаммирования с маркантом от секунд и полем длины открытого текста",
    "8": "4.5.2 и 4.5.5 Блочный шифр гаммирования с маркантом от числа месяца и полем длины открытого текста",
    "9": "4.5.3 и 4.5.6 Блочный шифр гаммирования с маркантом от дня недели и полем длины открытого текста",

    "10": "4.6.1.1 и 4.6.1.4 Блочный шифр гаммирования с маркантом от секунд и контрольной суммы, не зависящей от времени",
    "11": "4.6.1.2 и 4.6.1.5 Блочный шифр гаммирования с маркантом от числа месяца и контрольной суммы, не зависящей от времени",
    "12": "4.6.1.3 и 4.6.1.6 Блочный шифр гаммирования с маркантом от дня недели и контрольной суммы, не зависящей от времени",

    "13": "4.6.2.1 и 4.6.2.4 Блочный шифр гаммирования с маркантом от секунд и контрольной суммы, зависящей от времени",
    "14": "4.6.2.2 и 4.6.2.5 Блочный шифр гаммирования с маркантом от числа месяца и контрольной суммы, зависящей от времени",
    "15": "4.6.2.3 и 4.6.2.6 Блочный шифр гаммирования с маркантом от дня недели и контрольной суммы, зависящей от времени",

    "16": "4.7.1 и 4.7.2 Блочный шифр гаммирования с полем длины открытого текста и контрольной суммой",

    "17": "4.8.1.1 и 4.8.1.4 Блочный шифр гаммирования с маркантом от секунд, полем длины открытого текста и контрольной суммой, не зависящей от времени",
    "18": "4.8.1.2 и 4.8.1.5 Блочный шифр гаммирования с маркантом от числа месяца, полем длины открытого текста и контрольной суммой, не зависящей от времени",
    "19": "4.8.1.3 и 4.8.1.6 Блочный шифр гаммирования с маркантом от дня недели, полем длины открытого текста и контрольной суммой, не зависящей от времени",

    "20": "4.8.2.1 и 4.8.2.4 Блочный шифр гаммирования с маркантом от секунд, полем длины открытого текста и контрольной суммой, зависящей от времени",
    "21": "4.8.2.2 и 4.8.2.5 Блочный шифр гаммирования с маркантом от числа месяца, полем длины открытого текста и контрольной суммой, зависящей от времени",
    "22": "4.8.2.3 и 4.8.2.6 Блочный шифр гаммирования с маркантом от дня недели, полем длины открытого текста и контрольной суммой, зависящей от времени"
};

// Элементы DOM
const methodChoice = document.getElementById("methodChoice");
const methodDescription = document.getElementById("methodDescription");

// Слушатель изменения выбора метода шифрования
methodChoice.addEventListener("change", function () {
    const selectedValue = this.value;
    // Обновляем описание на основе выбранного значения
    methodDescription.textContent = descriptions[selectedValue] || "Описание отсутствует.";
});

// Инициализация: показываем описание для первого значения по умолчанию
window.onload = function () {
    const defaultValue = methodChoice.value;
    methodDescription.textContent = descriptions[defaultValue] || "Описание отсутствует.";
};

document.addEventListener("DOMContentLoaded", function () {
    const tabButtons = document.querySelectorAll(".tab-button");
    const tabContents = document.querySelectorAll(".tab-content");

    tabButtons.forEach(button => {
        button.addEventListener("click", function () {
            // Удаляем активные классы
            tabButtons.forEach(btn => btn.classList.remove("active"));
            tabContents.forEach(content => content.classList.remove("active"));

            // Добавляем активный класс на текущую вкладку и содержимое
            this.classList.add("active");
            const tabId = this.getAttribute("data-tab");
            document.getElementById(tabId).classList.add("active");
        });
    });

    // Показываем первую вкладку по умолчанию
    document.getElementById("encrypt").classList.add("active");
});
