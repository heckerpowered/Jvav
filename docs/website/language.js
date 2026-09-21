const translations = {
  en: {
    openNavigation: 'Open navigation', closeNavigation: 'Close navigation',
    changeColumn: 'Preview', appliedColumn: 'Applied',
    computeChange: 'Preview the result', applyChange: 'Apply change', tryAgain: 'Try again',
    entityInitial: 'Preview the computation while the entity stays unchanged.',
    entityComputed: 'Result previewed. The entity is still unchanged.',
    entityApplied: 'Change applied. Acceleration stays at 2.',
    copied: 'Copied', copiedLabel: 'Transition example copied',
    selected: 'Selected', selectedLabel: 'Example selected; use your browser to copy it',
    surfaceInitial: 'The surface is the current state.',
    surfaceComputed: 'New shape computed. The surface is unchanged.',
    surfaceApplied: 'Applied. The surface now has the computed shape.',
    previewShape: 'Preview a change', applyShape: 'Apply to state', startAgain: 'Start again',
  },
  zh: {
    openNavigation: '打开导航', closeNavigation: '关闭导航',
    changeColumn: '预览值', appliedColumn: '已应用',
    computeChange: '预览结果', applyChange: '应用变化', tryAgain: '再试一次',
    entityInitial: '先预览计算结果，实体保持不变。',
    entityComputed: '计算结果已预览，实体仍保持原样。',
    entityApplied: '变化已应用，加速度仍为 2。',
    copied: '已复制', copiedLabel: '已复制 transition 示例',
    selected: '已选中', selectedLabel: '示例已选中，可使用浏览器复制',
    surfaceInitial: '曲面表示当前状态。',
    surfaceComputed: '新形状已计算，原曲面保持不变。',
    surfaceApplied: '变化已应用，曲面已更新。',
    previewShape: '预览变化', applyShape: '应用到状态', startAgain: '再试一次',
  },
};

export const text = translations[document.documentElement.lang.startsWith('zh') ? 'zh' : 'en'];

const languagePicker = document.querySelector('.language-picker');
if (languagePicker) {
  const languageLinks = languagePicker.querySelectorAll('a[hreflang]');
  function updateLanguageLinks() {
    for (const link of languageLinks) {
      const destination = new URL(link.href);
      destination.search = window.location.search;
      destination.hash = window.location.hash;
      link.href = destination.href;
    }
  }
  languagePicker.addEventListener('toggle', updateLanguageLinks);
  window.addEventListener('hashchange', updateLanguageLinks);
  document.addEventListener('click', (event) => {
    if (!languagePicker.contains(event.target)) languagePicker.open = false;
  });
  document.addEventListener('keydown', (event) => {
    if (event.key !== 'Escape' || !languagePicker.open) return;
    languagePicker.open = false;
    languagePicker.querySelector('summary').focus();
  });
  updateLanguageLinks();
}
