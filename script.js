document.getElementById("button").onclick = function() {
    sendMessage();
};

document.getElementById("textbox").addEventListener("keydown", function(event) {
    if (event.key === "Enter") {
        event.preventDefault();
        sendMessage();
    }
});


function getTime() {
    const date = new Date();
    let ampm = "am";
    let hour = date.getHours();
    if (hour > 12) {
        ampm = "pm";
        hour -= 12;
    } else if (hour === 0) {
        hour = 12;
    }

    return `${hour}:${date.getMinutes()}:${date.getSeconds()}${ampm}`;
}

function sendMessage() {
    const text = document.getElementById("textbox").value;
    const chatBox = document.getElementById("chatbox");

    if (text.trim() !== "") {
        chatBox.innerHTML += `<div>${getTime()}: ${text}</div>`;
        chatBox.scrollTop = chatBox.scrollHeight;
        document.getElementById("textbox").value = "";
    }
};