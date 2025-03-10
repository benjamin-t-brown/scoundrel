// This file is for creating a scaffold around a game container
// that is exactly 640 x 480, including controls, and automatic
// resizing of the iframe to fit the scaffold.

const buttonCallbackStrings = {
  Up: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_UP)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_DOWN)',
  },
  Down: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_DOWN)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_UP)',
  },
  Left: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_LEFT)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_RIGHT)',
  },
  Right: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_RIGHT)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_LEFT)',
  },
  Confirm: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_SPACE)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_SPACE)',
  },
  Cancel: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_Z)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_Z)',
  },
  Shift: {
    down: 'Lib.handleButtonDown(Lib.BUTTON_SHIFT)',
    up: 'Lib.handleButtonUp(Lib.BUTTON_SHIFT)',
  },
};

const controlsDpadNormal = `
<div class="scaffold-dpad">
  <div class="scaffold-flex-row-center">
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Up.up}"
      ontouchstart="${buttonCallbackStrings.Up.up}"
      onmouseup="${buttonCallbackStrings.Up.down}"
      ontouchend="${buttonCallbackStrings.Up.down}"
      >↑</button>
  </div>
  <div class="scaffold-flex-row-space-between">
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Left.up}"
      ontouchstart="${buttonCallbackStrings.Left.up}"
      onmouseup="${buttonCallbackStrings.Left.down}"
      ontouchend="${buttonCallbackStrings.Left.down}"
      >←</button>
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Right.up}"
      ontouchstart="${buttonCallbackStrings.Right.up}"
      onmouseup="${buttonCallbackStrings.Right.down}"
      ontouchend="${buttonCallbackStrings.Right.down}"
      >→</button>  
  </div>
  <div class="scaffold-flex-row-center">
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Down.up}"
      ontouchstart="${buttonCallbackStrings.Down.up}"
      onmouseup="${buttonCallbackStrings.Down.down}"
      ontouchend="${buttonCallbackStrings.Down.down}"
      >↓</button>
  </div>
</div>
`;

const controlsDpadLr = `
<div class="scaffold-dpad">
  <div class="scaffold-flex-row-space-between">
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Left.up}"
      ontouchstart="${buttonCallbackStrings.Left.up}"
      onmouseup="${buttonCallbackStrings.Left.down}"
      ontouchend="${buttonCallbackStrings.Left.down}"
      >←</button>
    <button class="scaffold-button scaffold-button-direction"
      onmousedown="${buttonCallbackStrings.Right.up}"
      ontouchstart="${buttonCallbackStrings.Right.up}"
      onmouseup="${buttonCallbackStrings.Right.down}"
      ontouchend="${buttonCallbackStrings.Right.down}"
      >→</button>  
  </div>
</div>
`;

const controlsButtonsAb = `
<div class="scaffold-buttons">
  <div class="scaffold-flex-row-flex-end">
    <button class="scaffold-button scaffold-button-confirm"
      onmousedown="${buttonCallbackStrings.Confirm.up}"
      ontouchstart="${buttonCallbackStrings.Confirm.up}"
      onmouseup="${buttonCallbackStrings.Confirm.down}"
      ontouchend="${buttonCallbackStrings.Confirm.down}"
      >A</button>
  </div>
  <div class="scaffold-flex-row-center">
    <button class="scaffold-button scaffold-button-cancel"
      onmousedown="${buttonCallbackStrings.Cancel.up}"
      ontouchstart="${buttonCallbackStrings.Cancel.up}"
      onmouseup="${buttonCallbackStrings.Cancel.down}"
      ontouchend="${buttonCallbackStrings.Cancel.down}"
      >B</button>
  </div>
</div>
`;

const controlsButtonsAbShift = `
<div class="scaffold-buttons">
  <div class="scaffold-flex-row-center">
    <button class="scaffold-button scaffold-button-shift"
      onmousedown="${buttonCallbackStrings.Shift.up}"
      ontouchstart="${buttonCallbackStrings.Shift.up}"
      onmouseup="${buttonCallbackStrings.Shift.down}"
      ontouchend="${buttonCallbackStrings.Shift.down}"
      >X</button>
  </div>
  <div class="scaffold-flex-row-flex-end">
    <button class="scaffold-button scaffold-button-confirm"
      onmousedown="${buttonCallbackStrings.Confirm.up}"
      ontouchstart="${buttonCallbackStrings.Confirm.up}"
      onmouseup="${buttonCallbackStrings.Confirm.down}"
      ontouchend="${buttonCallbackStrings.Confirm.down}"
      >A</button>
  </div>
  <div class="scaffold-flex-row-center">
    <button class="scaffold-button scaffold-button-cancel"
      onmousedown="${buttonCallbackStrings.Cancel.up}"
      ontouchstart="${buttonCallbackStrings.Cancel.up}"
      onmouseup="${buttonCallbackStrings.Cancel.down}"
      ontouchend="${buttonCallbackStrings.Cancel.down}"
      >B</button>
  </div>
</div>
`;

let scaffoldOrientation = 'portrait';
let controlsVisible = true;
const mediaQuery = window.matchMedia('(orientation: portrait)');
function handleScreenSizeChange(e) {
  if (e.matches) {
    // Media query matches
    scaffoldOrientation = 'portrait';
    console.log('Screen is in portrait');
  } else {
    // Media query doesn't match
    scaffoldOrientation = 'landscape';
    console.log('Screen is in landscape');
  }
  if (!controlsVisible) {
    scaffoldHideControls();
  } else {
    scaffoldShowControls();
  }
}
handleScreenSizeChange(mediaQuery);
mediaQuery.addEventListener('change', handleScreenSizeChange);

// dpadLayout: normal, lr
// buttonsLayout: 'ab', 'abshift'
function scaffoldSetupBoard(dpadLayout, buttonsLayout) {
  const controlsDpad =
    dpadLayout === 'lr' ? controlsDpadLr : controlsDpadNormal;
  const controlsButtons =
    buttonsLayout === 'abshift' ? controlsButtonsAbShift : controlsButtonsAb;

  const scaffoldHtml = `
  <div class="scaffold-outer">
    <div id="top-bar-controls" class="top-bar-controls"></div>
    <div class="scaffold-inner">
      <div id="scaffold-controls-left" class="scaffold-controls">
        ${controlsDpad}
        <div class="scaffold-vertical-spacer"></div>
      </div>
      <div id="board" class="board scaffold-board">
        <canvas id="canvas" width="640" height="480"></canvas>
      </div>
      <div id="scaffold-controls-right" class="scaffold-controls">
        ${controlsButtons}
        <div class="scaffold-vertical-spacer"></div>
      </div>
      <div id="scaffold-controls-bottom">
        <div class="scaffold-controls">
          ${controlsDpad}
          <div class="scaffold-vertical-spacer"></div>
        </div>
        <div class="scaffold-controls">${controlsButtons}
        <div class="scaffold-vertical-spacer"></div>
        </div>
      </div>
    </div>
  </div>
  `;
  const game = document.getElementById('game');
  if (game) {
    game.innerHTML = scaffoldHtml;
  } else {
    console.error('Could not find "#game" element to append scaffold.');
  }
}

function scaffoldHideControls() {
  controlsVisible = false;
  console.log('scaffold hide controls');
  const controlsLeft = document.getElementById('scaffold-controls-left');
  const controlsRight = document.getElementById('scaffold-controls-right');
  const controlsBottom = document.getElementById('scaffold-controls-bottom');
  const board = document.getElementById('board');
  const canvas = document.getElementById('canvas');
  /** @type {any} */
  const scaffoldInner = document.querySelector('.scaffold-inner');

  if (controlsLeft) {
    controlsLeft.style.display = 'none';
    controlsLeft.style.visibility = 'hidden';
  }
  if (controlsRight) {
    controlsRight.style.display = 'none';
    controlsRight.style.visibility = 'hidden';
  }
  if (controlsBottom) {
    controlsBottom.style.visibility = 'hidden';
    if (Lib.getConfig().isFullScreen) {
      controlsBottom.style.setProperty('display', 'none', 'important');
    }
  }
  if (board) {
    board.style.width = '100%';
  }
  if (canvas) {
    if (scaffoldOrientation === 'landscape') {
      canvas.style.width = 'auto';
      if (Lib.getConfig().isFullScreen) {
        canvas.style.height = '100%';
      } else {
        canvas.style.height = 'calc(100% - 42 * 2px)';
      }
    }
  }
  if (scaffoldInner) {
    scaffoldInner.style.height = '100%';
  }
}

function scaffoldShowControls() {
  controlsVisible = true;
  const controlsLeft = document.getElementById('scaffold-controls-left');
  const controlsRight = document.getElementById('scaffold-controls-right');
  const controlsBottom = document.getElementById('scaffold-controls-bottom');
  const board = document.getElementById('board');
  const canvas = document.getElementById('canvas');
  /** @type {any} */
  const scaffoldInner = document.querySelector('.scaffold-inner');

  if (controlsLeft) {
    controlsLeft.style.display = '';
    controlsLeft.style.visibility = '';
  }
  if (controlsRight) {
    controlsRight.style.display = '';
    controlsRight.style.visibility = '';
  }
  if (controlsBottom) {
    controlsBottom.style.visibility = '';
    if (Lib.getConfig().isFullScreen) {
      controlsBottom.style.display = '';
    }
  }
  if (board) {
    board.style.width = '';
  }
  if (canvas) {
    canvas.style.height = '';
    canvas.style.width = '';
  }
  if (scaffoldInner) {
    scaffoldInner.style.height = '';
    scaffoldInner.style.transform = '';
  }
}
