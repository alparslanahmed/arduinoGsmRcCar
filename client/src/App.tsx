import type {Component} from 'solid-js';

import logo from './logo.svg';
import styles from './App.module.css';
import {JSX} from "solid-js";

const App: () => JSX.Element = () => {
    const Paho = window['Paho'];
    // Create a client instance
    const client = new Paho.MQTT.Client("162.55.170.36", 8080, "mqtt");

// set callback handlers
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

// connect the client
    client.connect({onSuccess: onConnect, useSSL: false});


// called when the client connects
    function onConnect() {
        // Once a connection has been made, make a subscription and send a message.
        console.log("onConnect");
        client.subscribe("World");
        const message = new Paho.MQTT.Message("Hello");
        message.destinationName = "World";
        client.send(message);
    }

// called when the client loses its connection
    function onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    }

// called when a message arrives
    function onMessageArrived(message) {
        console.log("onMessageArrived:" + message.payloadString);
    }

    return (
        <div class={styles.App}>
            <header class={styles.header}>
                <img src={logo} class={styles.logo} alt="logo"/>
                <p>
                    Edit <code>src/App.tsx</code> and save to reload.
                </p>
                <a
                    class={styles.link}
                    href="https://github.com/solidjs/solid"
                    target="_blank"
                    rel="noopener noreferrer"
                >
                    Learn Solid
                </a>
            </header>
        </div>
    );
};

export default App;
