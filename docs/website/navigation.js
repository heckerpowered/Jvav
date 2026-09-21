import { text } from './language.js';

const navigationToggle = document.querySelector('.menu-toggle');
const navigationLinks = document.querySelector('#navigation-links');

function setNavigationOpen(open) {
  navigationToggle.setAttribute('aria-expanded', String(open));
  navigationToggle.setAttribute('aria-label', open ? text.closeNavigation : text.openNavigation);
  navigationLinks.classList.toggle('is-open', open);
}

navigationToggle.addEventListener('click', () => {
  setNavigationOpen(navigationToggle.getAttribute('aria-expanded') !== 'true');
});
navigationLinks.addEventListener('click', (event) => {
  if (event.target.closest('a')) setNavigationOpen(false);
});
document.addEventListener('keydown', (event) => {
  if (event.key !== 'Escape' || navigationToggle.getAttribute('aria-expanded') !== 'true') return;
  setNavigationOpen(false);
  navigationToggle.focus();
});

document.documentElement.classList.add('has-js');
