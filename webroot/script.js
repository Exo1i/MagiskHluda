import {exec, toast} from 'kernelsu';

const statusIndicator = document.getElementById('serverStatus');
const statusText = document.getElementById('serverStatusText');
const toggleBtn = document.getElementById('toggleServerBtn');
const saveSettingsBtn = document.getElementById('saveSettingsBtn');
const portInput = document.getElementById('portInput');
const customParamsInput = document.getElementById('customParams');
const warningDialog = document.getElementById('warningDialog');
const cancelStop = document.getElementById('cancelStop');
const confirmStop = document.getElementById('confirmStop');

let isServerRunning = false;

const MODULE_PROP_PATH = '/data/adb/modules/magisk-hluda/module.prop';
const MODULE_SETTINGS_FILE = '/data/adb/modules/magisk-hluda/module.cfg';

async function initializeConfigFile() {
    try {
        const status = isServerRunning ? 1 : 0;
        const configContent = `port=${portInput.value}\nparameters=${customParamsInput.value}\nstatus=${status}`;
        await exec(`echo '${configContent}' > ${MODULE_SETTINGS_FILE}`);
        console.log('Created module.cfg with default values');
    } catch (error) {
        console.error('Error creating module.cfg:', error);
        toast('Failed to initialize config file');
    }
}

async function saveSettings() {
    try {
        // Validate port number
        const portNum = parseInt(portInput.value);
        if (isNaN(portNum) || portNum < 1 || portNum > 65535) {
            toast('Invalid port number. Using default port 27042');
            portInput.value = '27042';
        }

        // Check if config file exists
        const {errno} = await exec(`test -f ${MODULE_SETTINGS_FILE}`);

        if (errno !== 0) {
            await initializeConfigFile();
            return;
        }

        // Save port setting
        await exec(`sed -i "s/^port=.*/port=${portInput.value}/" ${MODULE_SETTINGS_FILE}`);

        // Save parameters setting (escape special characters)
        const escapedParams = customParamsInput.value
            .replace(/\//g, '\\/')
            .replace(/'/g, "\\'")
            .replace(/"/g, '\\"');
        await exec(`sed -i "s/^parameters=.*/parameters=${escapedParams}/" ${MODULE_SETTINGS_FILE}`);

        // Update status
        const currentStatus = isServerRunning ? 1 : 0;
        await exec(`sed -i "s/^status=.*/status=${currentStatus}/" ${MODULE_SETTINGS_FILE}`);

        toast('Settings saved successfully');
    } catch (error) {
        console.error('Failed to save settings:', error);
        toast(`Failed to save settings: ${error.message}`);
    }
}

async function loadSettings() {
    try {
        // Check if config file exists
        const {errno} = await exec(`test -f ${MODULE_SETTINGS_FILE}`);

        if (errno !== 0) {
            await initializeConfigFile();
            return;
        }

        // Read current settings
        const {stdout: portValue} = await exec(`grep "^port=" ${MODULE_SETTINGS_FILE} | cut -d= -f2`);
        const {stdout: paramsValue} = await exec(`grep "^parameters=" ${MODULE_SETTINGS_FILE} | cut -d= -f2-`);

        // Update input fields
        portInput.value = portValue.trim() || '27042';
        customParamsInput.value = paramsValue.trim() || '';

    } catch (error) {
        console.error('Failed to load settings:', error);
        toast(`Failed to load settings: ${error.message}`);
    }
}

async function updateModulePropStatus(running) {
    const status = running ? 'Running ✅' : 'Stopped ❌';
    try {
        await exec(`sed -i "s/^description=.*/description=[${status}]/" ${MODULE_PROP_PATH}`);
    } catch (error) {
        console.error('Failed to update module.prop:', error);
    }
}

function updateStatus(running) {
    if (isServerRunning === running) return;
    isServerRunning = running;

    statusIndicator.classList.toggle('status-running', running);
    statusIndicator.classList.toggle('status-stopped', !running);
    statusText.textContent = running ? 'Running' : 'Stopped';
    toggleBtn.textContent = running ? 'Stop Server' : 'Start Server';
    toggleBtn.classList.toggle('btn-danger', running);
    toggleBtn.classList.toggle('btn-primary', !running);

    updateModulePropStatus(running);
}

async function checkServerStatus() {
    try {
        const {errno} = await exec('pgrep -f florida');
        updateStatus(errno === 0);
    } catch (error) {
        console.error('Error checking server status:', error);
        updateStatus(false);
    }
}

async function startServer(port, customParams) {
    const baseCommand = `florida -D -l 0.0.0.0:${port}`;
    const fullCommand = customParams ? `${baseCommand} ${customParams}` : baseCommand;

    try {
        const {errno, stderr} = await exec(fullCommand);

        if (errno !== 0 || stderr.trim()) {
            // Either command failed or there was error output
            const errorMsg = stderr.trim() || 'Unknown error occurred';
            toast(`Failed to start server: ${errorMsg}`);
            updateStatus(false);
            return;
        }

        // If we got here, command succeeded
        setTimeout(checkServerStatus, 500);
        toast('Server started successfully');
    } catch (error) {
        toast(`Failed to start server: ${error.message}`);
        updateStatus(false);
    }
}

async function stopServer() {
    try {
        // frida doesn't respond to sigterm properly, so sending sigkill
        const {errno} = await exec('pkill -SIGKILL -f florida');
        if (errno === 0) {
            updateStatus(false);
            toast('Server stopped successfully');
        } else {
            throw new Error('Server not running');
        }
    } catch (error) {
        toast(`Failed to stop server: ${error.message}`);
    }
}

// Event Listeners
saveSettingsBtn.addEventListener('click', saveSettings);

toggleBtn.addEventListener('click', async () => {
    const port = portInput.value || '27042';
    const customParams = customParamsInput.value;

    if (!isServerRunning) {
        await saveSettings();
        await startServer(port, customParams);
    } else {
        warningDialog.style.display = 'flex';
    }
});

cancelStop.addEventListener('click', () => {
    warningDialog.style.display = 'none';
});

confirmStop.addEventListener('click', async () => {
    warningDialog.style.display = 'none';
    await saveSettings();
    await stopServer();
});

// Initialize
loadSettings().then(() => {
    checkServerStatus();
    setInterval(checkServerStatus, 500);
});