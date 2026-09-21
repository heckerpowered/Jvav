import { text } from './language.js';
import { visualDirections } from './visual-scenes.js';

export function initializeVisualArt(artwork, directionName) {
  const direction = visualDirections[directionName];
  const messages = direction[document.documentElement.lang.startsWith('zh') ? 'zh' : 'en'];
  const scene = artwork.querySelector('.state-art-scene');
  if (!scene.querySelector('[data-visual-scene]')) {
    scene.innerHTML = direction.render(`visual-${artwork.id}`);
  }
  artwork.dataset.visual = directionName;
  artwork.dataset.renderer = 'vector';
  artwork.setAttribute('aria-label', messages.name);
  const hero = artwork.closest('.hero');
  if (hero) hero.dataset.visual = directionName;

  const caption = artwork.querySelector('figcaption');
  const actionButton = artwork.querySelector('.surface-action');
  const resetButton = artwork.querySelector('.surface-reset');
  resetButton.setAttribute('aria-label', document.documentElement.lang.startsWith('zh') ? '重置视觉示意' : 'Reset the illustration');
  function setStage(stage) {
    artwork.dataset.stage = stage;
    caption.textContent = messages[stage];
    const actions = { initial: text.previewShape, computed: text.applyShape, applied: text.startAgain };
    actionButton.firstChild.textContent = `${actions[stage]} `;
  }
  actionButton.addEventListener('click', () => {
    const next = { initial: 'computed', computed: 'applied', applied: 'initial' };
    setStage(next[artwork.dataset.stage]);
  });
  resetButton.addEventListener('click', () => setStage('initial'));
  setStage('initial');
  artwork.querySelector('.state-art-controls').hidden = false;
}
