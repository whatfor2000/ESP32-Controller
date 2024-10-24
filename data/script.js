let Mode = ''; // Default to Manual Mode

function openTab(evt, modeValue) {
  const tabLinks = document.getElementsByClassName("tab-link");

  // Remove 'active' class from all tabs
  for (let i = 0; i < tabLinks.length; i++) {
    tabLinks[i].className = tabLinks[i].className.replace(" active", "");
  }

  // Add 'active' class to the clicked tab
  evt.currentTarget.className += " active";

  // Set Mode based on the selected tab
  Mode = modeValue;
}

// Set the default active tab

fetch("/getcalibation").then((R) => R.json()).then((data) => {
  document.getElementsByClassName("tab-link")[data.Mode].click();
  Mode = data.Mode == 0 ? 'manual' : 'random'
  const positions = ['m1open', 'm1close', 'm2open', 'm2close'];
  for (let i = 0; i <= 6; i++) {
    positions.push(`m3pos${i}`);
  }

  positions.forEach(pos => {
    if (data[pos] !== undefined) {
      document.getElementById(pos).value = data[pos];
    }
  });
});

function testCalibation(servo, position) {
  fetch(`/testcalibation?servo=${servo}&position=${position}`)
}

function start() {
  fetch(`/start/${Mode}`)
}

setInterval(() => {
  fetch('/data').then((e) => e.json()).then((data) => {
    document.getElementById("Time").innerHTML = data.Time
    document.getElementById("totalCount").innerHTML = data.totalCount
    document.getElementById("currentvalue").innerHTML = data.currentValue
    document.getElementById("status").innerHTML = data.Status ? 'ON' : 'OFF';
    document.getElementById("count0").innerHTML = data.positionCount[0]
    document.getElementById("count1").innerHTML = data.positionCount[1]
    document.getElementById("count2").innerHTML = data.positionCount[2]
    document.getElementById("count3").innerHTML = data.positionCount[3]
    document.getElementById("count4").innerHTML = data.positionCount[4]
    document.getElementById("count5").innerHTML = data.positionCount[5]
    document.getElementById("count6").innerHTML = data.positionCount[6]
  })
}, 500)