import {exec, toast} from 'kernelsu';

const statusIndicator = document.getElementById('serverStatus');
const statusText = document.getElementById('serverStatusText');
const toggleBtn = document.getElementById('toggleServerBtn');
const portInput = document.getElementById('portInput');
const customParamsInput = document.getElementById('customParams');
const warningDialog = document.getElementById('warningDialog');
const cancelStop = document.getElementById('cancelStop');
const confirmStop = document.getElementById('confirmStop');

let isServerRunning = false;

const MODULE_PROP_PATH = '/data/adb/modules/magisk-hluda/module.prop';

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

    // Update UI
    statusIndicator.classList.toggle('status-running', running);
    statusIndicator.classList.toggle('status-stopped', !running);
    statusText.textContent = running ? 'Running' : 'Stopped';
    toggleBtn.textContent = running ? 'Stop Server' : 'Start Server';

    // Update module.prop
    updateModulePropStatus(running);
}

async function checkServerStatus() {
    try {
        // More precise process matching with full path
        const {errno} = await exec('pgrep -f florida');
        updateStatus(errno === 0);
    } catch (error) {
        console.error('Error checking server status:', error);
        updateStatus(false);
    }
}

async function startServer(port, customParams) {
    const baseCommand = `florida -l 127.0.0.1:${port}`;
    const fullCommand = (customParams ? `${baseCommand} ${customParams}` : baseCommand) + ' >/dev/null 2>&1 &';

    try {
        await exec(fullCommand);
        setTimeout(checkServerStatus, 500);
    } catch (error) {
        toast(`Failed to start server: ${error.message}`);
        updateStatus(false);
    }
}

async function stopServer() {
    try {
        const {errno} = await exec('pkill -f florida');
        if (errno === 0) {
            updateStatus(false);
        } else {
            throw new Error('Server not running');
        }
    } catch (error) {
        toast(`Failed to stop server: ${error.message}`);
    }
}

// Handle stop warning modal
toggleBtn.addEventListener('click', async () => {
    const port = portInput.value || '27042';
    const customParams = customParamsInput.value;

    if (!isServerRunning) {
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
    await stopServer();
});

// Initial status check
checkServerStatus();
setInterval(checkServerStatus, 500);
