import {exec, toast} from 'kernelsu';

const statusIndicator = document.getElementById('serverStatus');
const statusText = document.getElementById('serverStatusText');
const toggleBtn = document.getElementById('toggleServerBtn');
const portInput = document.getElementById('portInput');
const customParamsInput = document.getElementById('customParams');

let isServerRunning = false;

function updateStatus(running) {
    isServerRunning = running;
    statusIndicator.classList.toggle('status-running', running);
    statusIndicator.classList.toggle('status-stopped', !running);
    statusText.textContent = running ? 'Running' : 'Stopped';
    toggleBtn.textContent = running ? 'Stop Server' : 'Start Server';
}

async function checkServerStatus() {
    try {
        // More precise process matching with full path
        const {errno} = await exec('pgrep -f "florida"');
        updateStatus(errno === 0);
    } catch (error) {
        console.error('Error checking server status:', error);
        updateStatus(false);
    }
}

async function startServer(port, customParams) {
    const baseCommand = `florida -l 127.0.0.1:${port}`;
    // Added output redirection and background operator
    const fullCommand = (customParams ? `${baseCommand} ${customParams}` : baseCommand) + ' >/dev/null 2>&1 &';

    try {
        await exec(fullCommand);
        // Verify server actually started after short delay
        setTimeout(checkServerStatus, 500);
    } catch (error) {
        toast(`Failed to start server: ${error.message}`);
        updateStatus(false);
    }
}

async function stopServer() {
    try {
        // More precise process killing with full path
        const {errno} = await exec('pkill -f "florida"');
        if (errno === 0) {
            updateStatus(false);
        } else {
            throw new Error('Server not running');
        }
    } catch (error) {
        toast(`Failed to stop server: ${error.message}`);
    }
}

toggleBtn.addEventListener('click', async () => {
    const port = portInput.value || '27042';
    const customParams = customParamsInput.value;

    if (!isServerRunning) {
        await startServer(port, customParams);
    } else {
        await stopServer();
    }
});

// Initial status check
checkServerStatus();
// Add periodic status checks
setInterval(checkServerStatus, 500);