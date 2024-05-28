window.onload = function () {};

document.onkeydown = fkey;
document.onkeypress = fkey;
document.onkeyup = fkey;

function fkey(e) {
    e = e || window.event;

    location.reload();
}
