function popcount_1_data(x) {
  var c = 0;
  var i = 0;
  for (i = 0; i < 64; i++) {
    c += (x & 1);
    x >>= 1;
  }
}

function popcount_4_control(x) {
  var c = 0;
  var i = 0;
  for (i = 0; i < 16; i++) {
    switch (x & 0xf) {
      case 0:
        c += 0;
        break;
      case 1:
        c += 1;
        break;
      case 2:
        c += 1;
        break;
      case 3:
        c += 2;
        break;
      case 4:
        c += 1;
        break;
      case 5:
        c += 2;
        break;
      case 6:
        c += 2;
        break;
      case 7:
        c += 3;
        break;
      case 8:
        c += 1;
        break;
      case 9:
        c += 2;
        break;
      case 10:
        c += 2;
        break;
      case 11:
        c += 3;
        break;
      case 12:
        c += 2;
        break;
      case 13:
        c += 3;
        break;
      case 14:
        c += 3;
        break;
      case 15:
        c += 4;
        break;
      default:
        break;
    }
    x >>= 4;
  }
  return c;
}

function popcount64_fast(x) {
  return x.toString(2).match(/1/g).length
}

function getRando() {
  return Math.random();
}

var random = getRando();
// var i = 0;
// for (i = 0; i < 10000; i++) {
//   var random1 = getRando();
//   popcount_1_data(random1);
//   popcount_4_control(random1);
//   popcount64_fast(random1);
// }

var start = performance.now();
popcount_1_data(random);
var end = performance.now();
var totalTime1 = end - start;
document.getElementById("para1").innerHTML="Total time for popcount 1 data: " + totalTime1;
console.log("Total time for popcount 1 data: " + totalTime1);

start = performance.now();
popcount_4_control(random);
end = performance.now();
var totalTime2 = end - start;
document.getElementById("para2").innerHTML="Total time for popcount 4 control: " + totalTime2;
console.log("Total time for popcount 4 control: " + totalTime2);
start = performance.now();
popcount64_fast(random);
end = performance.now();
var totalTime3 = end - start;
document.getElementById("para3").innerHTML="Total time for popcount 4 control: " + totalTime3;
console.log("Total time for popcount 64 fast: " + totalTime3);
