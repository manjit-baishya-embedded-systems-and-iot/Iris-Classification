let port, writer, reader;

document.getElementById('connect').addEventListener('click', async () => {
    const statusElement = document.getElementById('status');
    const receivedDataElement = document.getElementById('receivedData');

    try {
        // Request a port and open a connection
        port = await navigator.serial.requestPort();
        statusElement.textContent = 'Port selected. Opening...';

        // Configure and open the serial port
        await port.open({ baudRate: 115200 });
        statusElement.textContent = 'Connected to serial port';

        // Setup writer for sending data
        writer = port.writable.getWriter();

        // Read data from the serial port
        reader = port.readable.getReader();
        while (true) {
            const { value, done } = await reader.read();
            if (done) {
                // Allow the serial port to be closed later
                reader.releaseLock();
                break;
            }
            // Process and display the received data
            const receivedData = new TextDecoder().decode(value);
            receivedDataElement.value += receivedData;
            receivedDataElement.scrollTop = receivedDataElement.scrollHeight;  // Auto-scroll to the bottom
        }
    } catch (error) {
        statusElement.textContent = `Error: ${error.message}`;
        console.error('Failed to open serial port:', error);
    }
});

document.getElementById('sendButton').addEventListener('click', async () => {
    const statusElement = document.getElementById('status');
    const sendDataElement = document.getElementById('sendData');
    const dataToSend = sendDataElement.value;

    if (writer) {
        try {
            const encoder = new TextEncoder();
            await writer.write(encoder.encode(dataToSend + '\n'));
            statusElement.textContent = 'Data sent';
        } catch (error) {
            statusElement.textContent = `Error: ${error.message}`;
            console.error('Failed to send data:', error);
        }
    } else {
        statusElement.textContent = 'Serial port not connected';
    }
});
