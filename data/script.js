function openTab(evt) {
    const tabLinks = document.getElementsByClassName("tab-link");
    for (let i = 0; i < tabLinks.length; i++) {
      tabLinks[i].className = tabLinks[i].className.replace(" active", "");
    }
    evt.currentTarget.className += " active";
  }
  
  // Set the default active tab
  document.getElementsByClassName("tab-link")[0].click();