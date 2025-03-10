/** @type {any} */
var thisWindow = window;

function LIB() {
  const config = {
    startButtonEnabled: true,
    soundEnabled: true,
    shouldShowControls: true,
    shouldShowButtons: true,
    originalScale: false,
    gameStarted: false,
    autoPushStart: true,
    controlLayout: 'default', // default, overlay
    disableInput: false,
    isFullScreen: false,
  };

  let keyboardLayout = {};

  // SDL2 Button Codes for emcc/gcc compiler
  this.BUTTON_LEFT = 1073741904;
  this.BUTTON_RIGHT = 1073741903;
  this.BUTTON_UP = 1073741906;
  this.BUTTON_DOWN = 1073741905;
  this.BUTTON_SHIFT = 1073742049;
  this.BUTTON_ENTER = 13;
  this.BUTTON_SPACE = 32;
  this.BUTTON_Z = 122;

  const subscriptions = {
    onButtonDown: [],
    onButtonUp: [],
    onToggleModuleControls: [],
    onToggleSound: [],
    onSetVolume: [],
  };

  function getLabels() {
    return {
      okay: 'Okay',
    };
  }

  this.toggleSound = function () {
    if (config.soundEnabled) {
      console.log('[Lib] Disable sound');
      Module.ccall('disableSound');
    } else {
      console.log('[Lib] Enabled sound');
      Module.ccall('enableSound');
    }
    config.soundEnabled = !config.soundEnabled;
    this.invokeEvent('onToggleSound', config.soundEnabled);
  };
  this.setSoundEnabled = function (v) {
    config.soundEnabled = Boolean(v);
    this.invokeEvent('onToggleSound', config.soundEnabled);
  };
  this.toggleScale = function () {
    const board = document.getElementById('board');
    const canvas = document.getElementById('canvas');
    config.originalScale = !config.originalScale;
  };
  this.setScale = function (isOriginal) {
    config.originalScale = isOriginal;
  };
  this.toggleControls = function () {
    if (config.shouldShowControls) {
      this.hideControls();
    } else {
      this.showControls();
    }
    config.shouldShowControls = !config.shouldShowControls;
  };
  this.setControls = function (on) {
    if (on) {
      config.shouldShowControls = true;
      this.showControls();
    } else {
      config.shouldShowControls = false;
      this.hideControls();
    }
  };
  this.toggleButtons = function () {
    if (config.shouldShowButtons) {
      this.hideButtons();
    } else {
      this.showButtons();
    }
    config.shouldShowButtons = !config.shouldShowButtons;
  };
  this.showHelp = function (helpObj) {
    const localeCode = this.getLocalizationCode();
    const subUrl = helpObj[localeCode] || helpObj['en'];
    if (!helpObj[localeCode]) {
      console.error('No help URL found for locale', localeCode);
    }
    config.disableInput = true;

    if (subUrl) {
      let pathName = window.location.pathname;
      if (pathName.endsWith('.html')) {
        pathName = pathName.slice(0, pathName.lastIndexOf('/'));
      }
      if (pathName === '/') {
        pathName = '';
      }
      const url = (pathName + '/' + subUrl);
      console.log('[Lib] Show help url', url);

      const helpDiv = document.createElement('div');
      helpDiv.id = 'help';
      Object.assign(helpDiv.style, {
        position: 'fixed',
        left: '0',
        top: '0',
        width: '100%',
        height: '100%',
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: 'rgba(0,0,0,0.8)',
      });

      const helpDivInner = document.createElement('div');
      Object.assign(helpDivInner.style, {
        width: '100%',
        maxWidth: '640px',
        height: '100%',
        maxHeight: '480px',
        background: 'white',
      });
      helpDivInner.innerHTML = `<iframe id="help-iframe" style="width:100%;height:100%" src="${url}"></iframe>
      <div style="display:flex; width:100%; justify-content: center; margin-top:4px"><button onclick="Lib.hideHelp()">${
        getLabels().okay + ' ' + this.getEscapeKey().label
      }</button></div>`;

      document.body.appendChild(helpDiv);
      helpDiv.appendChild(helpDivInner);
    } else {
      console.error(
        'No help URL found to show help from this object:',
        helpObj
      );
    }
  };
  this.hideHelp = function () {
    const helpDiv = document.getElementById('help');
    if (helpDiv) {
      config.disableInput = false;
      helpDiv.remove();
    }
  };
  this.getLocalizationCode = function () {
    return 'en';
  };
  this.hideLoading = function () {
    const loading = document.getElementById('loading');
    if (loading) loading.style.display = 'none';
    const game = document.getElementById('game');
    if (game) game.style.display = 'flex';
    const error = document.getElementById('error');
    if (error) error.style.display = 'none';
  };
  this.showError = function () {
    const loading = document.getElementById('loading');
    if (loading) loading.style.display = 'none';
    const game = document.getElementById('game');
    if (game) game.style.display = 'none';
    const error = document.getElementById('error');
    if (error) error.style.display = 'flex';
  };
  this.hideControls = function () {
    const top = document.getElementById('top-bar-controls');
    if (top) top.style.display = 'none';
    const controls = document.getElementById('on-screen-controls');
    if (controls) controls.style.display = 'none';
  };
  this.showControls = function () {
    const top = document.getElementById('top-bar-controls');
    if (top) top.style.display = 'flex';
    const controls = document.getElementById('on-screen-controls');
    if (controls) controls.style.display = 'flex';
  };
  this.hideButtons = function () {
    const controls = document.getElementById('on-screen-controls');
    if (controls) controls.style.display = 'none';
    if (thisWindow.scaffoldHideControls) {
      thisWindow.scaffoldHideControls();
    }
  };
  this.showButtons = function () {
    const controls = document.getElementById('on-screen-controls');
    if (controls) controls.style.display = 'flex';
    if (thisWindow.scaffoldShowControls) {
      thisWindow.scaffoldShowControls();
    }
  };
  this.setDisplayModeFullScreen = function () {
    this.getConfig().isFullScreen = true;
    const canvas = document.getElementById('canvas');
    if (canvas) {
      Object.assign(canvas.style, {
        width: 'auto',
        height: '100%',
        maxWidth: 'unset',
        maxHeight: 'unset',
      });
    }
  };
  this.unsetDisplayModeFullscreen = function () {
    this.getConfig().isFullScreen = false;
    const canvas = document.getElementById('canvas');
    if (canvas) {
      Object.assign(canvas.style, {
        position: '',
        width: '',
        height: '',
        maxWidth: '',
        maxHeight: '',
        left: '',
        top: '',
      });
    }
  };
  this.handleButtonDown = function (key, ev) {
    if (config.disableInput) {
      return;
    }
    if (window.event) {
      window.event.preventDefault();
    }
    Module.ccall('setKeyDown', 'void', ['number'], [key]);
    this.invokeEvent('onButtonDown', key);
  };
  this.handleButtonUp = function (key) {
    if (config.disableInput) {
      return;
    }
    if (window.event) {
      window.event.preventDefault();
    }
    Module.ccall('setKeyUp', 'void', ['number'], [key]);
    this.invokeEvent('onButtonUp', key);
  };
  this.disableModuleControls = function () {
    Module.ccall('setKeyStatus', 'void', ['number'], [0]);
    this.invokeEvent('onToggleModuleControls', false);
  };
  this.enableModuleControls = function () {
    Module.ccall('setKeyStatus', 'void', ['number'], [1]);
    this.invokeEvent('onToggleModuleControls', true);
  };
  this.playerJoined = function () {};
  this.playerLeft = function () {};

  this.notifyParentFrame = function (action, payload) {
    if (window.parent) {
      console.log('[Lib] Notify parent', action);
      window.parent.postMessage(
        JSON.stringify({
          action,
          payload,
        }),
        '*'
      );
    } else {
      console.log('[Lib] No parent to notify.');
    }
  };

  this.notifyGameReady = function () {
    // wait just a bit to show the game so the audio doesn't glitch out (like it does for some reason for wasm stuff that has debug on)
    this.setWASMVolume(33);
    if (Lib.getConfig().autoPushStart) {
      setTimeout(() => {
        this.handleButtonDown(this.BUTTON_ENTER);
        this.handleButtonUp(this.BUTTON_ENTER);
      }, 500);
    }
    this.notifyParentFrame('GAME_READY', {});
  };

  this.notifyGameStarted = function () {
    this.notifyParentFrame('GAME_STARTED', {});
    config.gameStarted = true;
  };

  this.notifyGameCompleted = function (...results) {
    this.notifyParentFrame(
      'GAME_CONCLUDED',
      results.length > 1 ? results : results[0]
    );
    config.gameStarted = false;
  };

  this.notifyGameCancelled = function () {
    this.notifyParentFrame('GAME_CANCELLED', {});
    config.gameStarted = false;
  };

  this.notifyRPGScript = function (scriptSrc) {
    this.notifyParentFrame('RUN_RPGSCRIPT', {
      scriptSrc,
    });
  };

  this.notifyGameCustom = function (args) {
    this.notifyParentFrame('GAME_CUSTOM', args);
  };

  this.notifyEscapePressed = function () {
    this.notifyParentFrame('ESCAPE_PRESSED', {});
  };

  this.notifyEscapeUnpressed = function () {
    this.notifyParentFrame('ESCAPE_UNPRESSED', {});
  };

  // number between 0 and 100
  this.setWASMVolume = function (pct) {
    if (isNaN(pct)) {
      pct = 33;
    }
    Module.ccall('setVolume', 'void', ['number'], [pct]);
    this.invokeEvent('onSetVolume', pct);
  };

  this.getConfig = function () {
    return config;
  };

  this.setControlLayout = function (layout) {
    keyboardLayout = layout;
  };

  this.getActionKey = function () {
    if (keyboardLayout.confirm) {
      if (keyboardLayout.confirm.label.includes('(X)')) {
        return {
          ...keyboardLayout.confirm,
          label: '(Space)',
        };
      } else {
        return keyboardLayout.confirm;
      }
    } else {
      return {
        key: [' ', 'x', 'X'],
        label: '(Space)',
      };
    }
  };
  this.isActionKey = function (key) {
    return this.getActionKey().key.includes(key);
  };
  this.isConfirmKey = this.isActionKey;
  this.getConfirmKey = this.getActionKey;

  this.getCancelKey = function () {
    return (
      keyboardLayout.cancel || {
        key: ['z'],
        label: '(Z)',
      }
    );
  };
  this.isCancelKey = function (key) {
    return this.getCancelKey().key.includes(key);
  };

  this.getAuxKey = function () {
    if (keyboardLayout.shift) {
      if (keyboardLayout.shift.label.includes('(X)')) {
        return {
          ...keyboardLayout.shift,
          label: '(Shift)',
        };
      } else {
        return keyboardLayout.shift;
      }
    } else {
      return {
        key: ['Shift'],
        label: '(Shift)',
      };
    }
  };
  this.isAuxKey = function (key) {
    return this.getAuxKey().key.includes(key);
  };

  this.getUpKey = function () {
    return {
      key: ['ArrowUp'],
      label: '(Up)',
    };
  };
  this.isUpKey = function (key) {
    return ['ArrowUp'].includes(key);
  };

  this.getDownKey = function () {
    return (
      keyboardLayout.down || {
        key: ['ArrowDown'],
        label: '(Dn)',
      }
    );
  };
  this.isDownKey = function (key) {
    return this.getDownKey().key.includes(key);
  };

  this.getLeftKey = function () {
    return (
      keyboardLayout.left || {
        key: ['ArrowLeft'],
        label: '(Left)',
      }
    );
  };
  this.isLeftKey = function (key) {
    return this.getLeftKey().key.includes(key);
  };

  this.getRightKey = function () {
    return (
      keyboardLayout.right || {
        key: ['ArrowRight'],
        label: '(Right)',
      }
    );
  };
  this.isRightKey = function (key) {
    return this.getRightKey().key.includes(key);
  };

  this.getSkipKey = function () {
    return (
      keyboardLayout.skip || {
        key: ['Backspace', 'Escape'],
        label: '(Backspace)',
      }
    );
  };
  this.isSkipKey = function (key) {
    return this.getSkipKey().key.includes(key);
  };

  this.getEscapeKey = function () {
    return (
      keyboardLayout.escape || {
        key: ['Escape'],
        label: '(Esc)',
      }
    );
  };
  this.isEscapeKey = function (key) {
    return ['Escape'].includes(key);
  };

  this.isCabinet = function () {
    return isArcadeCabinet;
  };

  this.createTopBarButton = function ({ id, svgSrc1, svgSrc2, onClick, type }) {
    let showSvg1 = true;
    const button = document.createElement('button');
    button.id = id;
    button.onclick = ev => {
      showSvg1 = !showSvg1;
      if (showSvg1) {
        img.src = svgSrc1;
      } else {
        img.src = svgSrc2 || svgSrc1;
      }
      onClick(ev);
    };

    if (type == 'cancel') {
      // button.style.backgroundColor = '#A93B3B';
      button.classList.add('cancel-button');
    }

    if (isArcadeCabinet) {
      button.style.display = 'none';
    }

    const img = document.createElement('img');
    img.src = svgSrc1;
    img.alt = id;
    img.style.width = '24px';

    button.appendChild(img);
    return button;
  };

  this.subscribe = function (eventName, callback) {
    if (subscriptions[eventName]) {
      subscriptions[eventName].push(callback);
    }
  };

  this.unsubscribe = function (eventName, callback) {
    if (subscriptions[eventName]) {
      subscriptions[eventName] = subscriptions[eventName].filter(
        cb => cb !== callback
      );
    }
  };

  this.invokeEvent = function (eventName, ...args) {
    if (subscriptions[eventName]) {
      subscriptions[eventName].forEach(cb => cb(...args));
    }
  };
}

var gamepadButtonsState = new Map();
var simulateKeyPress = (buttonIndex, isDown) => {
  var gamepadButtonIndexToKey = {
    0: 'x',
    1: 'z',
    2: 'c',
    3: 'y',
    4: 'left bumper',
    5: 'right bumper',
    6: 'shift',
    7: 'right trigger',
    8: 'Backspace',
    9: ' ',
    10: 'left stick',
    11: 'right stick',
    12: 'ArrowUp',
    13: 'ArrowDown',
    14: 'ArrowLeft',
    15: 'ArrowRight',
    16: 'home',
  };
  var gamepadButtonToLibKey = {
    0: Lib.BUTTON_SPACE,
    1: Lib.BUTTON_SHIFT,
    2: Lib.BUTTON_UP,
    3: 'y',
    4: 'left bumper',
    5: 'right bumper',
    6: 'shift',
    7: 'right trigger',
    8: 'Backspace',
    9: ' ',
    10: 'left stick',
    11: 'right stick',
    12: Lib.BUTTON_UP,
    13: Lib.BUTTON_DOWN,
    14: Lib.BUTTON_LEFT,
    15: Lib.BUTTON_RIGHT,
    16: 'home',
  };

  var key = gamepadButtonIndexToKey[buttonIndex];
  if (!key) {
    return;
  }

  var simulatedEvent = new KeyboardEvent(isDown ? 'keydown' : 'keyup', {
    key,
  });
  window.dispatchEvent(simulatedEvent);
  if (isDown) {
    Lib.handleButtonDown(gamepadButtonToLibKey[buttonIndex], simulatedEvent);
  } else {
    Lib.handleButtonUp(gamepadButtonToLibKey[buttonIndex]);
  }
};

var getAxisButtonPress = (axisValue, button1I, button2I) => {
  if (axisValue < -0.75) {
    return button1I;
  } else if (axisValue > 0.75) {
    return button2I;
  }
  return null;
};

var updateGamePad = gamepad => {
  var buttonsToUnpress = [];
  var buttonsToSkipUnpress = [];
  for (var [i, button] of gamepad.buttons.entries()) {
    var pctPressed = button.value * 100;
    if (button.pressed && pctPressed > 50) {
      buttonsToSkipUnpress.push(i);
      if (!gamepadButtonsState.get(i)) {
        gamepadButtonsState.set(i, true);
        simulateKeyPress(i, true);
      }
    } else {
      buttonsToUnpress.push(i);
    }
  }

  for (var [i, axisValue] of gamepad.axes.entries()) {
    let localButtonsToUnpress = [];
    // left stick x
    if (i === 0) {
      var leftButtonI = 14;
      var rightButtonI = 15;

      var buttonI = getAxisButtonPress(axisValue, leftButtonI, rightButtonI);
      if (buttonI !== null) {
        if (!gamepadButtonsState.get(buttonI)) {
          gamepadButtonsState.set(buttonI, true);
          simulateKeyPress(buttonI, true);
        }
        buttonsToSkipUnpress.push(buttonI);
        localButtonsToUnpress.push(
          buttonI === leftButtonI ? rightButtonI : leftButtonI
        );
      } else if (buttonI === null) {
        localButtonsToUnpress.push(leftButtonI);
        localButtonsToUnpress.push(rightButtonI);
      }
    }
    // left stick y
    if (i === 1) {
      var upButtonI = 12;
      var downButtonI = 13;

      var buttonI = getAxisButtonPress(axisValue, upButtonI, downButtonI);
      if (buttonI !== null) {
        if (!gamepadButtonsState.get(buttonI)) {
          gamepadButtonsState.set(buttonI, true);
          simulateKeyPress(buttonI, true);
        }
        buttonsToSkipUnpress.push(buttonI);
        localButtonsToUnpress.push(
          buttonI === upButtonI ? downButtonI : upButtonI
        );
      } else if (buttonI === null) {
        localButtonsToUnpress.push(upButtonI);
        localButtonsToUnpress.push(downButtonI);
      }
    }
    for (var j of localButtonsToUnpress) {
      if (gamepadButtonsState.get(i)) {
        buttonsToUnpress.push(j);
      }
    }
  }

  for (var i of buttonsToUnpress) {
    if (buttonsToSkipUnpress.includes(i)) {
      continue;
    }
    if (gamepadButtonsState.get(i)) {
      // button was released
      gamepadButtonsState.set(i, false);
      simulateKeyPress(i, false);
    }
  }
};

var Lib = (thisWindow.Lib = new LIB());
var queryString = thisWindow.location.search;
var params = new URLSearchParams(queryString);
var expand = false; //params.get('cabinet');
var isArcadeCabinet = params.get('cabinet');
var gamepadInterval;
var captureGamePad = isArcadeCabinet ? false : true;

var Module = {
  arguments: ['--wait', '--nointro'],
  jsLoaded: function () {
    Module.preRun[0]();
  },
  preRun: [
    function () {
      clearTimeout(thisWindow.loadTimeout);
    },
  ],
  postRun: [
    function () {
      Lib.hideLoading();
      const shouldMute = params.get('muted');
      if (shouldMute === 'true') {
        // HACK: delayed because the WASM is sometimes not ready to accept the call
        // to mute it
        setTimeout(() => {
          Lib.getConfig().soundEnabled = true;
          Lib.toggleSound();
        }, 300);
      }

      if (isArcadeCabinet) {
        Lib.disableModuleControls();
      }
      // This should be notified from inside the program itself
      // Lib.notifyGameReady();
    },
  ],
  canvas: (function () {
    const canvas = document.getElementById('canvas');
    if (canvas) {
      canvas.addEventListener(
        'webglcontextlost',
        function (e) {
          console.error(
            '[Lib] WebGL context lost. You will need to reload the page.'
          );
          Lib.showError();
          e.preventDefault();
        },
        false
      );
      // const isArcadeCabinet = expand === 'true';
      if (isArcadeCabinet) {
        canvas.style.border = 'unset';
      }
    }

    return canvas;
  })(),
  onAbort: function () {
    console.error('[Lib] Program encountered an unknown error.');
    Lib.showError();
  },
  totalDependencies: 0,
  ccall: function (...args) {},
};

window.addEventListener(
  'keydown',
  ev => {
    if (ev.key === 'F5') {
      window.location.reload();
      ev.preventDefault();
    } else if (ev.key === 'Escape') {
      ev.preventDefault();
      ev.stopPropagation();
      Lib.hideHelp();
      if (!ev.repeat) {
        Lib.notifyEscapePressed();
      }
    }
  },
  true
);

window.addEventListener(
  'keyup',
  ev => {
    if (ev.key === 'Escape') {
      ev.preventDefault();
      ev.stopPropagation();
      Lib.notifyEscapeUnpressed();
    }
  },
  true
);

window.addEventListener('message', event => {
  try {
    const data = JSON.parse(event.data);
    console.log('%cgot iframe action ' + data?.action, 'color:#aaF');
    if (data.action === 'HIDE_CONTROLS') {
      Lib.getConfig().shouldShowControls = true;
      Lib.toggleControls();
    } else if (data.action === 'SHOW_CONTROLS') {
      Lib.getConfig().shouldShowControls = false;
      Lib.toggleControls();
    } else if (data.action === 'SEND_CONTROL_LAYOUT') {
      Lib.setControlLayout(data);
    } else if (data.action === 'SCALE_ORIGINAL') {
      Lib.setScale(true);
      const canvas = document.getElementById('canvas');
      if (canvas && isArcadeCabinet) {
        canvas.style.border = 'unset';
      }
    } else if (data.action === 'SCALE_WINDOW') {
      Lib.setScale(false);
      const canvas = document.getElementById('canvas');
      if (canvas && isArcadeCabinet) {
        canvas.style.border = '1px solid white';
      }
    } else if (data.action === 'MUTE_AUDIO') {
      Lib.getConfig().soundEnabled = true;
      Lib.toggleSound();
    } else if (data.action === 'UNMUTE_AUDIO') {
      Lib.getConfig().soundEnabled = false;
      Lib.toggleSound();
    } else if (data.action === 'SET_VOLUME') {
      Lib.setWASMVolume(Math.floor(data.payload * 100));
    } else if (data.action === 'BUTTON_DOWN') {
      Lib.handleButtonDown(data.payload);
    } else if (data.action === 'BUTTON_UP') {
      Lib.handleButtonUp(data.payload);
    } else if (data.action === 'DISABLE_CONTROLS') {
      Lib.disableModuleControls();
    } else if (data.action === 'PLAYER_JOINED') {
      Lib.playerJoined();
    } else if (data.action === 'PLAYER_LEFT') {
      Lib.playerLeft();
    } else if (data.action === 'CUSTOM_DATA') {
      if (thisWindow.onCustomData) {
        thisWindow.onCustomData(JSON.parse(data.payload));
      } else {
        console.error(
          'Error, cannot CUSTOM_DATA no "onCustomData" function found on window.  Payload:',
          data.payload
        );
      }
    } else if (data.action === 'BEGIN_GAME') {
      if (thisWindow.start) {
        // TODO if it's touchscreen, unset fullscreen and show controls
        // Lib.unsetDisplayModeFullscreen();
        captureGamePad = true;
        thisWindow.start(data.payload);
      } else {
        console.error(
          'Error, cannot BEGIN_GAME no "start" function found on window'
        );
      }
    }
  } catch (e) {
    console.warn('[Lib] Error on postMessage handler', e, event.data);
  }
});

// required for wasm to grab keyboard controls
setInterval(() => {
  if (Lib.getConfig().gameStarted) {
    window.focus();
  }
}, 500);

async function localInit() {
  verify();
  thisWindow.loadTimeout = setTimeout(function () {
    console.error('[Lib] Content took too long to load.');
    Lib.showError();
  }, 60000);

  try {
    await thisWindow.init({
      params,
      gameType: params.get('gameType'),
    });
  } catch (e) {
    console.error(
      '[Lib] Error calling window.init function, is it defined for this program?'
    );
    throw e;
  }
}

function verify() {
  if (!thisWindow.init) {
    console.error('[Lib] no `init` found.');
  }
  if (!thisWindow.start) {
    console.error('[Lib] no `start` found.');
  }
}

if (!isArcadeCabinet) {
  Lib.getConfig().shouldShowControls = true;
  const startButton = document.getElementById('start');
  Lib.getConfig().startButtonEnabled = false;
  if (startButton) startButton.style.display = 'none';
  window.addEventListener('load', () => {
    console.log('[Lib] loaded Lib', expand);
    // Lib.toggleControls();
    Lib.setControls(true);
    Lib.toggleScale();
    const toggleScaleElem = document.getElementById('toggle-scale');
    if (toggleScaleElem) toggleScaleElem.style.display = 'none';
    const toggleSoundElem = document.getElementById('toggle-sound');
    if (toggleSoundElem) toggleSoundElem.style.display = 'none';
    const toggleControlsElem = document.getElementById('toggle-controls');
    if (toggleControlsElem) toggleControlsElem.style.display = 'none';
    const toggleControls2Elem = document.getElementById('toggle-controls2');
    if (toggleControls2Elem) toggleControls2Elem.style.display = 'none';
  });
} else {
  Lib.setDisplayModeFullScreen();
  Lib.hideButtons();
}

const tapToStart = params.get('tap');
if (tapToStart === 'true') {
  const div = document.createElement('div');
  const loadingElem = document.getElementById('loading');
  if (loadingElem) {
    loadingElem.style.display = 'none';
  }
  thisWindow.onTapToStart = function () {
    window.removeEventListener('mousedown', thisWindow.onTapToStart);
    window.removeEventListener('touchstart', thisWindow.onTapToStart);
    if (loadingElem) {
      loadingElem.style.display = 'flex';
    }
    div.style.display = 'none';
    localInit();
  };
  div.innerHTML = 'Tap to Start';
  div.className = 'tap-to-start';
  document.body.appendChild(div);
  thisWindow.addEventListener('mousedown', thisWindow.onTapToStart);
  thisWindow.addEventListener('touchstart', thisWindow.onTapToStart);
} else {
  thisWindow.addEventListener('load', localInit);
}

gamepadInterval = setInterval(() => {
  if (Lib.getConfig().gameStarted || !isArcadeCabinet) {
    for (const gamepad of navigator.getGamepads()) {
      if (!gamepad) {
        continue;
      }
      updateGamePad(gamepad);
    }
  }
}, 16);
