document.querySelectorAll('[data-capability-examples]').forEach((examples) => {
  const tabList = examples.querySelector('[role="tablist"]');
  const tabs = Array.from(tabList.querySelectorAll('[role="tab"]'));
  const panels = tabs.map((tab) => document.getElementById(tab.getAttribute('aria-controls')));

  function selectTab(selectedTab) {
    tabs.forEach((tab, index) => {
      const selected = tab === selectedTab;
      tab.setAttribute('aria-selected', String(selected));
      tab.tabIndex = selected ? 0 : -1;
      panels[index].inert = !selected;
      panels[index].setAttribute('aria-hidden', String(!selected));
      panels[index].tabIndex = selected ? 0 : -1;
    });
  }

  tabs.forEach((tab, index) => {
    panels[index].setAttribute('role', 'tabpanel');
    panels[index].setAttribute('aria-labelledby', tab.id);
    tab.addEventListener('click', () => selectTab(tab));
    tab.addEventListener('keydown', (event) => {
      let nextIndex;
      switch (event.key) {
        case 'ArrowLeft': nextIndex = (index + tabs.length - 1) % tabs.length; break;
        case 'ArrowRight': nextIndex = (index + 1) % tabs.length; break;
        case 'Home': nextIndex = 0; break;
        case 'End': nextIndex = tabs.length - 1; break;
        default: return;
      }
      event.preventDefault();
      selectTab(tabs[nextIndex]);
      tabs[nextIndex].focus();
    });
  });

  selectTab(tabs[0]);
  examples.classList.add('is-enhanced');
  tabList.hidden = false;
});
