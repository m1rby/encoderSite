function updateTooltip() {
    const tooltipText = document.getElementById("tooltip-text");
    const method = document.getElementById("method").value;

    switch (method) {
        case "Ш4.1.2":
            tooltipText.innerHTML = "Ш4.1.2: Метод с блоком 16 байт, модулем 69 и гаммой для латиницы или кириллицы.";
            break;
        case "Ш4.2.4":
            tooltipText.innerHTML = "Ш4.2.4: Метод с метками по времени (секунды, день, неделя).";
            break;
        case "Ш4.3.2":
            tooltipText.innerHTML = "Ш4.3.2: Метод с полем длины открытого текста.";
            break;
        case "Ш4.4.2":
            tooltipText.innerHTML = "Ш4.4.2: Метод с полем контрольной суммы.";
            break;
        default:
            tooltipText.innerHTML = "Выберите метод шифрования для получения информации.";
    }
}

// Добавляем обработчики событий для показа и скрытия подсказки
document.querySelector('.tooltip').addEventListener('mouseenter', () => {
    document.getElementById("tooltip-text").style.display = 'block';
});

document.querySelector('.tooltip').addEventListener('mouseleave', () => {
    document.getElementById("tooltip-text").style.display = 'none';
});
