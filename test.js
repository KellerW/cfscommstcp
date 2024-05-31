const net = require('net');

function main() {
    // Create a socket and connect to the server on localhost and port 1234
    const client = net.createConnection({ port: 1234, host: '127.0.0.1' }, () => {
        console.log('Connected to server!');
        
        // Send 'AAAAAAAA\n'
        client.write('AAAAAAAA\n');
        setTimeout(() => {
            // Send 'BBBBBBBB\n' after 1 second
            client.write('BBBBBBBB\n');
            setTimeout(() => {
                // Send 'CCCCCCCC\n' after another 1 second
                client.write('CCCCCCCC\n');
            }, 1000);
        }, 1000);
    });

    // Event handler for receiving data
    client.on('data', (data) => {
        console.log('Received data from server:', data.toString());
    });

    client.on('end', () => {
        console.log('Disconnected from server');
    });

    client.on('error', (err) => {
        console.error('Error:', err.message);
    });
}

// Execute the main function
main();