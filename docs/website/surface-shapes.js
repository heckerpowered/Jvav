function flowPoint(along, across) {
  const horizontal = along * Math.sqrt(1 - across * across * .5);
  const depth = across * Math.sqrt(1 - along * along * .5);
  const height = .62 * Math.sin(horizontal * 2.5 - .2) +
    .72 * horizontal * depth + .26 * depth * depth - .08;
  return [horizontal * 4.1, height, depth * 1.8];
}

function orbitPoint(along, across) {
  const angle = -(along + 1) * Math.PI;
  const radius = 2.5 + across * .68;
  const height = .22 * Math.sin(angle * 2) + .22 * across * Math.cos(angle) - .05;
  return [Math.cos(angle) * radius * 1.22, height, Math.sin(angle) * radius * .64];
}

function foldPoint(along, across) {
  const crease = along + across * .22;
  const height = .95 * (Math.sqrt(crease * crease + .025) - .60) + across * .14;
  return [along * 3.6, height, across * 1.45];
}

function ripplePoint(along, across) {
  const horizontal = along * Math.sqrt(1 - across * across * .5);
  const depth = across * Math.sqrt(1 - along * along * .5);
  const radius = Math.hypot(horizontal, depth);
  const height = .48 * Math.cos(radius * 6.2) * Math.exp(-radius * radius * .7) - .05;
  return [horizontal * 3.6, height, depth * 2.05];
}

export const surfaceShapes = {
  flow: { point: flowPoint, closed: false },
  orbit: { point: orbitPoint, closed: true },
  fold: { point: foldPoint, closed: false },
  ripple: { point: ripplePoint, closed: false },
};
