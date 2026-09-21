function scene(content) {
  return `<svg class="visual-scene-svg" data-visual-scene viewBox="0 0 1200 500" xmlns="http://www.w3.org/2000/svg" aria-hidden="true">${content}</svg>`;
}

function imprintScene(prefix) {
  const lines = Array.from({ length: 43 }, (_, index) => {
    const height = 80 + index * 8;
    return `<path d="M290 ${height} C435 ${height - 78} 596 ${height + 82} 895 ${height - 34}"/>`;
  }).join('');
  return scene(`
    <defs>
      <filter id="${prefix}-shadow" x="-20%" y="-25%" width="140%" height="160%"><feDropShadow dx="0" dy="13" stdDeviation="14" flood-color="#77664e" flood-opacity=".13"/></filter>
      <pattern id="${prefix}-paper" width="7" height="7" patternUnits="userSpaceOnUse"><path d="M0 1h2M4 5h1" stroke="#827660" stroke-width=".7" opacity=".12"/></pattern>
      <clipPath id="${prefix}-print"><path d="M330 279C334 143 434 116 555 133C678 150 807 89 865 189C926 294 797 373 668 373C516 373 326 414 330 279Z"/></clipPath>
      <mask id="${prefix}-old"><rect width="1200" height="500" fill="white"/><rect x="665" y="171" width="168" height="130" fill="black"/></mask>
      <clipPath id="${prefix}-patch"><rect x="665" y="171" width="168" height="130"/></clipPath>
    </defs>
    <g transform="rotate(-4 600 255)">
      <rect x="251" y="58" width="698" height="387" rx="2" fill="#fffdf7" filter="url(#${prefix}-shadow)"/>
      <rect x="251" y="58" width="698" height="387" rx="2" fill="url(#${prefix}-paper)"/>
      <g fill="none" stroke="#ada797" stroke-width=".8"><path d="M281 93h20m-10-10v20M899 410h20m-10-10v20"/></g>
      <g class="visual-current" clip-path="url(#${prefix}-print)" fill="none" stroke="#1d4542" stroke-width="3.1">
        <g mask="url(#${prefix}-old)">${lines}</g>
        <g class="imprint-change" clip-path="url(#${prefix}-patch)">${lines}</g>
      </g>
      <rect x="873" y="377" width="16" height="16" fill="#ce5337"/>
      <g class="visual-proposal imprint-proposal">
        <rect x="650" y="157" width="198" height="158" fill="#fffcf4" fill-opacity=".85" stroke="#cb5339" stroke-width="1"/>
        <g clip-path="url(#${prefix}-patch)" fill="none" stroke="#c45336" stroke-width="2.5">${lines}</g>
        <path d="M647 146v-12m-6 6h12M851 326v12m-6-6h12" stroke="#cb5339" fill="none"/>
      </g>
    </g>
  `);
}

function opticsScene(prefix) {
  const mirror = (horizontal, vertical, angle, depth) => `
    <g transform="translate(${horizontal} ${vertical}) rotate(${angle})">
      <path d="M-87 0H87L104 ${depth}H-70Z" fill="url(#${prefix}-glass)" stroke="#b3d6d2" stroke-opacity=".24" stroke-width=".9"/>
      <path d="M-87 0H87L88 5H-86Z" fill="#bce8df" fill-opacity=".13"/>
      <path d="M-70 ${depth}H104" stroke="#d4e9eb" stroke-opacity=".15" stroke-width=".7"/>
      <path d="M-87 0H87" stroke="url(#${prefix}-edge)" stroke-width="2"/>
    </g>`;
  const beam = (path, gradient) => `
    <g fill="none" stroke="url(#${prefix}-${gradient})" stroke-linejoin="round" stroke-linecap="round">
      <path d="${path}" stroke-width="30" opacity=".28" filter="url(#${prefix}-atmosphere)"/>
      <path d="${path}" stroke-width="12" opacity=".75" filter="url(#${prefix}-glow)"/>
      <path d="${path}" stroke-width="3.2"/>
      <path d="${path}" stroke="#fcfff2" stroke-width="1" opacity=".8"/>
    </g>`;
  const reflections = [[325, 190], [600, 350], [865, 175]].map(([horizontal, vertical]) => `
    <circle cx="${horizontal}" cy="${vertical}" r="18" fill="#e0ffe8" opacity=".6" filter="url(#${prefix}-glow)"/>
    <ellipse cx="${horizontal}" cy="${vertical}" rx="13" ry="2" fill="#f2ffe8" opacity=".9"/>
    <circle cx="${horizontal}" cy="${vertical}" r="3.8" fill="#fffef0"/>
  `).join('');
  return scene(`
    <defs>
      <linearGradient id="${prefix}-incoming" gradientUnits="userSpaceOnUse" x1="85" y1="310" x2="865" y2="175"><stop stop-color="#ffd99a" stop-opacity=".2"/><stop offset=".13" stop-color="#ffefc2"/><stop offset=".55" stop-color="#eaffca"/><stop offset="1" stop-color="#b6f8e9"/></linearGradient>
      <linearGradient id="${prefix}-outgoing" gradientUnits="userSpaceOnUse" x1="865" y1="175" x2="1115" y2="285"><stop stop-color="#c0ffe9"/><stop offset=".6" stop-color="#a9f4e4"/><stop offset="1" stop-color="#9cdded" stop-opacity=".12"/></linearGradient>
      <linearGradient id="${prefix}-glass" x1="0" y1="0" x2=".3" y2="1"><stop stop-color="#b1d7dc" stop-opacity=".16"/><stop offset=".4" stop-color="#b0d1d8" stop-opacity=".045"/><stop offset="1" stop-color="#c6f5dd" stop-opacity=".13"/></linearGradient>
      <linearGradient id="${prefix}-edge" gradientUnits="userSpaceOnUse" x1="-87" y1="0" x2="87" y2="0"><stop stop-color="#a4d3c7" stop-opacity=".12"/><stop offset=".5" stop-color="#edfdeb" stop-opacity=".9"/><stop offset="1" stop-color="#9ac5c9" stop-opacity=".15"/></linearGradient>
      <radialGradient id="${prefix}-halo"><stop stop-color="#7aaf99" stop-opacity=".13"/><stop offset="1" stop-color="#59988e" stop-opacity="0"/></radialGradient>
      <filter id="${prefix}-glow" filterUnits="userSpaceOnUse" x="0" y="0" width="1200" height="500"><feGaussianBlur stdDeviation="5"/></filter>
      <filter id="${prefix}-atmosphere" filterUnits="userSpaceOnUse" x="0" y="0" width="1200" height="500"><feGaussianBlur stdDeviation="17"/></filter>
    </defs>
    <ellipse cx="598" cy="270" rx="487" ry="213" fill="url(#${prefix}-halo)"/>
    ${mirror(325, 190, 1.8, -74)}
    ${mirror(600, 350, -1.6, 70)}
    <g class="optics-turning">${mirror(865, 175, -4.8, -74)}</g>
    ${beam('M85 310 325 190 600 350 865 175', 'incoming')}
    <g class="visual-current optics-current">${beam('M865 175 1115 285', 'outgoing')}</g>
    <g class="visual-proposal optics-proposal" fill="none">
      <path d="M865 175 1115 285" stroke="#cbfff3" stroke-width="2" stroke-dasharray="5 8"/>
      <circle cx="1115" cy="285" r="6" stroke="#cbfff3" stroke-width="1.5"/>
    </g>
    <g class="visual-proposal optics-mirror-proposal" fill="none" stroke="#d1f8e7" stroke-opacity=".45" stroke-width="1" stroke-dasharray="3 5">
      <path transform="translate(865 175) rotate(-4.8)" d="M-87 0H87L104 -74H-70Z"/>
    </g>
    ${reflections}
  `);
}

function pixelsScene() {
  const letters = [
    ['11111','00010','00010','00010','10010','10010','01100'],
    ['10001','10001','10001','10001','10001','01010','00100'],
    ['01110','10001','10001','11111','10001','10001','10001'],
    ['10001','10001','10001','10001','10001','01010','00100'],
  ];
  const fixed = [];
  const moving = [];
  const proposal = [];
  for (const [letterIndex, rows] of letters.entries()) {
    for (const [row, columns] of rows.entries()) {
      for (const [column, filled] of [...columns].entries()) {
        if (filled !== '1') continue;
        const left = 232 + (letterIndex * 6 + column) * 32;
        const top = 163 + row * 32;
        const cell = `<rect x="${left}" y="${top}" width="26" height="26" rx=".8"/>`;
        if (row === 0 && letterIndex !== 1) {
          moving.push(cell);
          proposal.push(cell);
        } else fixed.push(cell);
      }
    }
  }
  const guides = Array.from({ length: 24 }, (_, index) => `<path d="M${229 + index * 32} 151v244"/>`).join('') +
    Array.from({ length: 8 }, (_, index) => `<path d="M229 ${160 + index * 32}h736"/>`).join('');
  return scene(`
    <g fill="none" stroke="#a1afb1" stroke-opacity=".2" stroke-width=".6">${guides}</g>
    <g class="visual-current pixels-current">
      <g fill="#14262b">${fixed.join('')}</g>
      <g class="pixels-moving" fill="#bdd835">${moving.join('')}</g>
    </g>
    <g class="visual-proposal pixels-proposal" fill="none" stroke="#425762" stroke-width="1.3" stroke-dasharray="3 3">${proposal.join('')}</g>
    <path d="M232 415h114" stroke="#14262b" stroke-width="2"/>
    <path d="M861 415h106" stroke="#14262b" stroke-width=".7"/>
    <path d="m961 411 6 4-6 4" fill="none" stroke="#14262b" stroke-width=".7"/>
  `);
}

export const visualDirections = {
  imprint: {
    render: imprintScene,
    en: { name: 'Imprint', initial: 'The printed pattern is the current state.', computed: 'The next impression is ready. The print is unchanged.', applied: 'Applied. The new impression is part of the print.' },
    zh: { name: '纸面印记', initial: '纸上的印记表示当前状态。', computed: '新的印记已计算，纸面保持不变。', applied: '变化已应用，新的印记已落在纸上。' },
  },
  optics: {
    render: opticsScene,
    en: { name: 'Optics', initial: 'The light traces the current state.', computed: 'A new path is ready. The light has not moved.', applied: 'Applied. The light follows the new path.' },
    zh: { name: '光学叠影', initial: '光路表示当前状态。', computed: '新的光路已计算，光还未改变方向。', applied: '变化已应用，光沿着新的路径传播。' },
  },
  pixels: {
    render: pixelsScene,
    en: { name: 'Pixels', initial: 'The tiles form the current state.', computed: 'The next arrangement is ready. The tiles have not moved.', applied: 'Applied. The tiles are in their new positions.' },
    zh: { name: '像素排布', initial: '字块组成当前状态。', computed: '新的排列已计算，字块还未移动。', applied: '变化已应用，字块已经就位。' },
  },
};
