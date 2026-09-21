import { text } from './language.js';

const lab = document.querySelector('.transition-lab');
const advanceButton = document.querySelector('#advance-demo');
const resetButton = document.querySelector('#reset-demo');
const status = document.querySelector('#demo-status');
const positionValue = document.querySelector('#position-value');
const velocityValue = document.querySelector('#velocity-value');
const positionChange = document.querySelector('#position-change');
const velocityChange = document.querySelector('#velocity-change');
const changeColumn = document.querySelector('#change-column');
let stage = 'initial';

function showStage(nextStage) {
  stage = nextStage;
  lab.dataset.stage = stage;
  const computed = stage !== 'initial';
  const applied = stage === 'applied';
  positionValue.textContent = applied ? '15' : '10';
  velocityValue.textContent = applied ? '5' : '3';
  positionChange.textContent = computed ? '15' : '—';
  velocityChange.textContent = computed ? '5' : '—';
  changeColumn.textContent = applied ? text.appliedColumn : text.changeColumn;
  const labels = { initial: text.computeChange, computed: text.applyChange, applied: text.tryAgain };
  advanceButton.firstChild.textContent = `${labels[stage]} `;
  const messages = {
    initial: text.entityInitial,
    computed: text.entityComputed,
    applied: text.entityApplied,
  };
  status.textContent = messages[stage];
}

document.querySelector('.demo-controls').hidden = false;
advanceButton.addEventListener('click', () => {
  const next = { initial: 'computed', computed: 'applied', applied: 'initial' };
  showStage(next[stage]);
});
resetButton.addEventListener('click', () => showStage('initial'));

const copyButton = document.querySelector('#copy-example');
copyButton.addEventListener('click', async () => {
  const example = document.querySelector('#transition-source').textContent;
  try {
    await navigator.clipboard.writeText(example);
    copyButton.querySelector('span').textContent = text.copied;
    copyButton.setAttribute('aria-label', text.copiedLabel);
  } catch {
    const selection = window.getSelection();
    const range = document.createRange();
    range.selectNodeContents(document.querySelector('#transition-source'));
    selection.removeAllRanges();
    selection.addRange(range);
    copyButton.querySelector('span').textContent = text.selected;
    copyButton.setAttribute('aria-label', text.selectedLabel);
  }
});
