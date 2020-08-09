'use strict';

function getNowTime() {
    let date = new Date();
    let year = date.getFullYear();
    let month = date.getMonth() + 1;
    let day = date.getDate();
    let hour = date.getHours() < 10 ? "0" + date.getHours() : date.getHours();
    let minute = date.getMinutes() < 10 ? "0" + date.getMinutes() : date.getMinutes();
    let second = date.getSeconds() < 10 ? "0" + date.getSeconds() : date.getSeconds();
    let milsec = date.getMilliseconds();
    return year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second + "." + milsec;
}

function format_integer(number, base) {
    let arr = [];
    while (number > 0) {
        arr.push(number % base);
        number = Math.floor(number / base);
    }
    arr.reverse();
    return arr.join('');
}

function unit_test() {
    console.log(format_integer(50, 2));
    console.log(format_integer(50, 8));
}

unit_test();
