function openTab(evt) {
  const tabLinks = document.getElementsByClassName("tab-link");
  for (let i = 0; i < tabLinks.length; i++) {
    tabLinks[i].className = tabLinks[i].className.replace(" active", "");
  }
  evt.currentTarget.className += " active";
}

// Set the default active tab
document.getElementsByClassName("tab-link")[0].click();

fetch("/getcalibation").then((R) => R.json()).then((data) => {
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