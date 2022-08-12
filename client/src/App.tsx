import type {Component} from 'solid-js';

import logo from './logo.svg';
import styles from './App.module.css';
import {JSX} from "solid-js";

const App: () => JSX.Element = () => {
    const Paho = window['Paho'];
    // Create a client instance
    const client = new Paho.MQTT.Client("162.55.170.36", 9001, "/");

// set callback handlers
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

// connect the client
    client.connect({onSuccess: onConnect, useSSL: false});


// called when the client connects
    function onConnect() {
        // Once a connection has been made, make a subscription and send a message.
        client.subscribe("deneme_kanal");
        const message = new Paho.MQTT.Message("w");
        message.destinationName = "deneme_kanal";
        client.send(message);
    }

    function send(key){
        const message = new Paho.MQTT.Message(key);
        message.destinationName = "deneme_kanal";
        client.send(message);
        console.log('MESSAGE', key)
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

    //event listener
    window.addEventListener("keydown", onKeyDown, false);
    window.addEventListener("keyup", onKeyUp, false);

    function onKeyDown(event) {
        var keyCode = event.keyCode;
        switch (keyCode) {
            case 68: //d
            case 83: //s
            case 65: //a
            case 87: //w
        }
    }

    function onKeyUp(event) {
        var keyCode = event.keyCode;
        console.log(keyCode)
        switch (keyCode) {
            case 68: //d
                send("d");
                break;
            case 83: //s
                send("s");
                break;
            case 65: //a
                send("a");
                break;
            case 87: //w
                send("w");
                break;

        }
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
