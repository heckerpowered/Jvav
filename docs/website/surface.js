import { text } from './language.js';
import { surfaceShapes } from './surface-shapes.js';
import { visualDirections } from './visual-scenes.js';
import { initializeVisualArt } from './visual-art.js';

function initializeArtwork(artwork) {
  const requestedVisual = artwork.dataset.visual || (artwork.dataset.shape ? null : new URLSearchParams(window.location.search).get('visual'));
  if (Object.hasOwn(visualDirections, requestedVisual)) {
    initializeVisualArt(artwork, requestedVisual);
    return;
  }
  const requestedShape = artwork.dataset.shape || new URLSearchParams(window.location.search).get('shape') || 'flow';
  const shapeName = Object.hasOwn(surfaceShapes, requestedShape) ? requestedShape : 'flow';
  const shape = surfaceShapes[shapeName];
  artwork.dataset.shape = shapeName;
  const canvas = artwork.querySelector('canvas');
  const outline = artwork.querySelector('svg');
  const caption = artwork.querySelector('figcaption');
  const actionButton = artwork.querySelector('.surface-action');
  const resetButton = artwork.querySelector('.surface-reset');
  const reducedMotion = window.matchMedia('(prefers-reduced-motion: reduce)');
  const vectorNamespace = 'http://www.w3.org/2000/svg';
  const yaw = -.28;
  const camera = new Float32Array([
    .175 * Math.cos(yaw), .36 * Math.sin(yaw), -.07 * Math.sin(yaw),
    0, .68, -.07,
    .175 * Math.sin(yaw), -.36 * Math.cos(yaw), .07 * Math.cos(yaw),
  ]);

  function surfacePoint(along, across, change) {
    const point = shape.point(along, across);
    const alongDistance = Math.abs(along - .20);
    const distanceFromChange = shape.closed ? Math.min(alongDistance, 2 - alongDistance) : alongDistance;
    const distance = distanceFromChange ** 2 / .24 + ((across + .18) ** 2) / .7;
    point[1] += change * .75 * Math.exp(-distance);
    return point;
  }

  function surfaceNormal(along, across, change) {
    const point = surfacePoint(along, across, change);
    const forward = surfacePoint(along + .001, across, change);
    const side = surfacePoint(along, across + .001, change);
    const tangent = forward.map((value, index) => (value - point[index]) * 1000);
    const crosswise = side.map((value, index) => (value - point[index]) * 1000);
    return [
      crosswise[1] * tangent[2] - crosswise[2] * tangent[1],
      crosswise[2] * tangent[0] - crosswise[0] * tangent[2],
      crosswise[0] * tangent[1] - crosswise[1] * tangent[0],
    ];
  }

  function project(point) {
    const horizontal = camera[0] * point[0] + camera[3] * point[1] + camera[6] * point[2];
    const vertical = camera[1] * point[0] + camera[4] * point[1] + camera[7] * point[2] - .07;
    return [(horizontal + 1) * 600, (1 - vertical) * 215];
  }

  function curvePath(points, close = false) {
    return points.map((point, index) => {
      const [horizontal, vertical] = project(point);
      return `${index === 0 ? 'M' : 'L'}${horizontal.toFixed(2)} ${vertical.toFixed(2)}`;
    }).join(' ') + (close ? ' Z' : '');
  }

  function makePath(parent) {
    const path = document.createElementNS(vectorNamespace, 'path');
    parent.append(path);
    return path;
  }

  const surfaceEdge = makePath(outline.querySelector('.state-art-contours'));
  const previewGroup = outline.querySelector('.state-art-preview');
  const previewArea = outline.querySelector('.state-art-preview-area');
  const fallbackSurface = outline.querySelector('.state-art-fallback');

  function drawOutline(change, previewOpacity) {
    const border = [];
    for (let index = 0; index <= 80; index++) border.push(surfacePoint(-1 + index / 40, -1, change));
    for (let index = 1; index <= 80; index++) border.push(surfacePoint(1, -1 + index / 40, change));
    for (let index = 79; index >= 0; index--) border.push(surfacePoint(-1 + index / 40, 1, change));
    for (let index = 79; index > 0; index--) border.push(surfacePoint(-1, -1 + index / 40, change));
    let boundary = curvePath(border, true);
    if (shape.closed) {
      const inner = [];
      const outer = [];
      for (let index = 0; index <= 160; index++) {
        inner.push(surfacePoint(-1 + index / 80, -1, change));
        outer.push(surfacePoint(-1 + index / 80, 1, change));
      }
      boundary = curvePath(inner, true) + ' ' + curvePath(outer, true);
    }
    fallbackSurface.setAttribute('d', boundary);
    surfaceEdge.setAttribute('d', boundary);

    const lift = (1 - change) * .24;
    function previewPoint(along, across) {
      const point = surfacePoint(along, across, 1);
      point[1] += lift;
      return point;
    }
    const patch = Array.from({ length: 81 }, (_, index) => {
      const angle = index / 80 * Math.PI * 2;
      return previewPoint(.20 + Math.cos(angle) * .54, -.18 + Math.sin(angle) * .69);
    });
    previewArea.setAttribute('d', curvePath(patch, true));
    previewGroup.setAttribute('opacity', String(previewOpacity));
  }

  function createRenderer() {
    // This illustration redraws on demand; keep its last frame across compositing updates.
    const graphics = canvas.getContext('webgl', {
      alpha: true, antialias: true, premultipliedAlpha: true, preserveDrawingBuffer: true,
    });
    if (!graphics) return null;

    function compileShader(type, source) {
      const shader = graphics.createShader(type);
      graphics.shaderSource(shader, source);
      graphics.compileShader(shader);
      if (!graphics.getShaderParameter(shader, graphics.COMPILE_STATUS)) {
        graphics.deleteShader(shader);
        throw new Error('The surface shader could not be compiled.');
      }
      return shader;
    }

    const vertexShader = compileShader(graphics.VERTEX_SHADER, `
      attribute vec3 oldPosition;
      attribute vec3 newPosition;
      attribute vec3 oldNormal;
      attribute vec3 newNormal;
      attribute vec2 coordinates;
      uniform float change;
      uniform float lift;
      uniform mat3 camera;
      varying vec3 normal;
      varying vec3 position;
      varying vec2 surfaceCoordinates;
      void main() {
        position = mix(oldPosition, newPosition, change);
        position.y += lift;
        normal = normalize(mix(oldNormal, newNormal, change));
        surfaceCoordinates = coordinates;
        vec3 projected = camera * position;
        projected.y -= .07;
        gl_Position = vec4(projected, 1.0);
      }
    `);
    const fragmentShader = compileShader(graphics.FRAGMENT_SHADER, `
      precision mediump float;
      varying vec3 normal;
      varying vec3 position;
      varying vec2 surfaceCoordinates;
      uniform float previewOpacity;
      uniform float previewLayer;
      void main() {
        vec3 direction = normalize(normal);
        vec3 light = normalize(vec3(-.6, 1.2, .8));
        vec3 eye = normalize(vec3(.12, 1.5, 2.2));
        vec3 reflection = reflect(-eye, direction);
        float facing = max(dot(direction, eye), 0.0);
        float rim = pow(1.0 - facing, 3.0);
        float diffuse = .66 + .34 * max(dot(direction, light), 0.0);
        float tone = smoothstep(-.85, .8, position.x * .15 - position.z * .39);
        vec3 base = mix(vec3(.27, .22, .76), vec3(.12, .56, .93), tone);
        vec3 color = base * diffuse;
        float studioLight = exp(-pow((reflection.z + .35) / .22, 2.0));
        float edgeLight = pow(max(dot(direction, normalize(light + eye)), 0.0), 65.0);
        color = mix(color, vec3(.84, .94, 1.0), studioLight * .84);
        color += vec3(.45, .53, .64) * edgeLight + vec3(.20, .25, .38) * rim;
        float alpha = 1.0;
        if (previewLayer > .5) {
          vec2 patch = (surfaceCoordinates - vec2(.20, -.18)) / vec2(.54, .69);
          float radius = length(patch);
          if (radius > 1.0) discard;
          float border = smoothstep(.86, 1.0, radius);
          color = mix(color, vec3(.56, .74, 1.0), .65);
          alpha = previewOpacity * (.38 + .12 * studioLight + .18 * border);
        }
        gl_FragColor = vec4(color, alpha);
      }
    `);
    const program = graphics.createProgram();
    graphics.attachShader(program, vertexShader);
    graphics.attachShader(program, fragmentShader);
    graphics.linkProgram(program);
    graphics.deleteShader(vertexShader);
    graphics.deleteShader(fragmentShader);
    if (!graphics.getProgramParameter(program, graphics.LINK_STATUS)) {
      graphics.deleteProgram(program);
      throw new Error('The surface renderer could not be linked.');
    }
    graphics.useProgram(program);

    const lengthSegments = 120;
    const widthSegments = 36;
    const vertices = [];
    const indices = [];
    for (let row = 0; row <= widthSegments; row++) {
      const across = -1 + row / widthSegments * 2;
      for (let column = 0; column <= lengthSegments; column++) {
        const along = -1 + column / lengthSegments * 2;
        vertices.push(...surfacePoint(along, across, 0), ...surfacePoint(along, across, 1));
        vertices.push(...surfaceNormal(along, across, 0), ...surfaceNormal(along, across, 1), along, across);
        if (row === widthSegments || column === lengthSegments) continue;
        const index = row * (lengthSegments + 1) + column;
        const nextRow = index + lengthSegments + 1;
        indices.push(index, nextRow, index + 1, index + 1, nextRow, nextRow + 1);
      }
    }
    const vertexBuffer = graphics.createBuffer();
    graphics.bindBuffer(graphics.ARRAY_BUFFER, vertexBuffer);
    graphics.bufferData(graphics.ARRAY_BUFFER, new Float32Array(vertices), graphics.STATIC_DRAW);
    const attributes = [['oldPosition', 3, 0], ['newPosition', 3, 3], ['oldNormal', 3, 6], ['newNormal', 3, 9], ['coordinates', 2, 12]];
    for (const [name, size, offset] of attributes) {
      const location = graphics.getAttribLocation(program, name);
      graphics.enableVertexAttribArray(location);
      graphics.vertexAttribPointer(location, size, graphics.FLOAT, false, 14 * 4, offset * 4);
    }
    const indexBuffer = graphics.createBuffer();
    graphics.bindBuffer(graphics.ELEMENT_ARRAY_BUFFER, indexBuffer);
    graphics.bufferData(graphics.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), graphics.STATIC_DRAW);
    graphics.uniformMatrix3fv(graphics.getUniformLocation(program, 'camera'), false, camera);
    const changeLocation = graphics.getUniformLocation(program, 'change');
    const liftLocation = graphics.getUniformLocation(program, 'lift');
    const previewLocation = graphics.getUniformLocation(program, 'previewLayer');
    const opacityLocation = graphics.getUniformLocation(program, 'previewOpacity');
    graphics.enable(graphics.DEPTH_TEST);
    graphics.clearColor(0, 0, 0, 0);

    return (change, previewOpacity) => {
      const density = Math.min(window.devicePixelRatio || 1, 2);
      const width = Math.round(canvas.clientWidth * density);
      const height = Math.round(canvas.clientHeight * density);
      if (canvas.width !== width || canvas.height !== height) {
        canvas.width = width;
        canvas.height = height;
        graphics.viewport(0, 0, width, height);
      }
      graphics.uniform1f(changeLocation, change);
      graphics.uniform1f(liftLocation, 0);
      graphics.uniform1f(previewLocation, 0);
      graphics.disable(graphics.BLEND);
      graphics.depthMask(true);
      graphics.clear(graphics.COLOR_BUFFER_BIT | graphics.DEPTH_BUFFER_BIT);
      graphics.drawElements(graphics.TRIANGLES, indices.length, graphics.UNSIGNED_SHORT, 0);
      if (previewOpacity <= 0) return;
      graphics.uniform1f(changeLocation, 1);
      graphics.uniform1f(liftLocation, (1 - change) * .24);
      graphics.uniform1f(previewLocation, 1);
      graphics.uniform1f(opacityLocation, previewOpacity);
      graphics.enable(graphics.BLEND);
      graphics.blendFuncSeparate(graphics.SRC_ALPHA, graphics.ONE_MINUS_SRC_ALPHA, graphics.ONE, graphics.ONE_MINUS_SRC_ALPHA);
      graphics.depthMask(false);
      graphics.drawElements(graphics.TRIANGLES, indices.length, graphics.UNSIGNED_SHORT, 0);
    };
  }

  let renderSurface = null;
  try { renderSurface = createRenderer(); } catch { canvas.hidden = true; }
  artwork.dataset.renderer = renderSurface ? 'webgl' : 'svg';

  let currentChange = 0;
  let currentPreview = 0;
  let animation = 0;
  let targetChange = 0;
  let targetPreview = 0;

  function draw(change, previewOpacity) {
    currentChange = change;
    currentPreview = previewOpacity;
    if (renderSurface) renderSurface(change, previewOpacity);
    drawOutline(change, previewOpacity);
  }

  function animateTo(change, previewOpacity) {
    cancelAnimationFrame(animation);
    targetChange = change;
    targetPreview = previewOpacity;
    if (reducedMotion.matches) {
      draw(change, previewOpacity);
      return;
    }
    const initialChange = currentChange;
    const initialPreview = currentPreview;
    const start = performance.now();
    function frame(now) {
      const progress = Math.min((now - start) / 600, 1);
      const eased = 1 - (1 - progress) ** 3;
      draw(initialChange + (change - initialChange) * eased, initialPreview + (previewOpacity - initialPreview) * eased);
      if (progress < 1) animation = requestAnimationFrame(frame);
    }
    animation = requestAnimationFrame(frame);
  }

  function setStage(stage) {
    artwork.dataset.stage = stage;
    const messages = {
      initial: text.surfaceInitial,
      computed: text.surfaceComputed,
      applied: text.surfaceApplied,
    };
    const actions = { initial: text.previewShape, computed: text.applyShape, applied: text.startAgain };
    caption.textContent = messages[stage];
    actionButton.firstChild.textContent = `${actions[stage]} `;
    animateTo(stage === 'applied' ? 1 : 0, stage === 'computed' ? 1 : 0);
  }

  actionButton.addEventListener('click', () => {
    const next = { initial: 'computed', computed: 'applied', applied: 'initial' };
    setStage(next[artwork.dataset.stage]);
  });
  resetButton.addEventListener('click', () => setStage('initial'));
  reducedMotion.addEventListener('change', () => {
    if (!reducedMotion.matches) return;
    cancelAnimationFrame(animation);
    draw(targetChange, targetPreview);
  });
  canvas.addEventListener('webglcontextlost', (event) => {
    event.preventDefault();
    renderSurface = null;
    canvas.hidden = true;
    artwork.dataset.renderer = 'svg';
  });
  new ResizeObserver(() => draw(currentChange, currentPreview)).observe(canvas);
  draw(0, 0);
  artwork.querySelector('.state-art-controls').hidden = false;
}

document.querySelectorAll('.state-art').forEach(initializeArtwork);
