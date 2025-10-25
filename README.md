# White Hand – The Hand That Speaks ✋

**White Hand** is a smart glove project that translates sign language gestures into readable text using Arduino-based wireless communication.  
It was developed as part of the course *"Applications of Telecommunication Devices"* at Aristotle University of Thessaloniki.

---

## 📖 Overview

The system consists of three types of nodes communicating via an **ALOHA-based wireless network**:

- **Transmitter Node (Glove):**  
  Reads flex sensor data from the user’s fingers and encodes gestures into specific letters.

- **Central Node (Base Station):**  
  Receives packets from transmitters and routes them to the correct receivers.

- **Receiver Node:**  
  Displays the translated letter on an LCD screen.

---

## ⚙️ Hardware Components

| Component | Quantity | Description |
|------------|-----------|-------------|
| Arduino Nano / UNO | 3 | One for each node |
| Flex Sensors | 2+ | For thumb and index finger (expandable) |
| RF Modules (433 MHz / 425 MHz) | 3 | For wireless transmission |
| LCD Display | 1 | Connected to the receiver node |
| Push Button | 1 | For selecting receiver address |
| LEDs | 3 | Status indication on central node |
| Resistors, Jumpers, Breadboards | Various | For connections |

---

## 🔌 Circuit Diagrams

You can find the connection schematics inside [`/docs/wiring_diagrams`](./docs/wiring_diagrams).  
Each node has a dedicated diagram and `.ino` file under `/src/`.

---

## 🧠 Code Structure

src/
│
├── transmitter/
│ └── transmitter.ino
├── receiver/
│ └── receiver.ino
├── central_node/
│ └── central_node.ino
└── calibration/
└── calibration.ino


Each `.ino` file includes initialization, setup, and loop functions for its respective node.  
Refer to the **Technical Report** in `/report/` for detailed pseudocode and explanation.

---

## 🧰 Setup & Calibration

1. Upload the correct `.ino` file to each Arduino board.
2. Before using the glove, run the **Calibration** sketch to record sensor value ranges.
3. Adjust detection thresholds based on your hand and flex sensor readings.
4. Use the push button to change the receiver address.
5. The system uses **baud rate 9600**, **frequency 425 MHz**, **power 20 dBm**, **modulation FSK_Rb125Fd125**.

---

## 📡 Communication Logic

- Each transmitted packet includes:
[DATA][TRANSMITTER_ID][RECEIVER_ID]

Example: `157` means letter “A” from node 5 to receiver 7.

- The central node routes packets and blinks LEDs:
- 🔵 Blue → Letter received
- ⚪ White → Idle state
- 🔴 Red → Error or disconnection

---

## 🎥 Demo Videos

- 📺 **Promotional Video:** [YouTube Link](https://youtu.be/EPcsYgDs9gM)  
- 🧤 **Demo in Action:** [YouTube Link](https://youtu.be/o5tppeNglUs)

---

## 🧾 Documentation

You can find the full **Technical Report (PDF)** in the `/report` directory.

> **Authors:**  
> - Ilianna Kampouridou – iliakamp@ece.auth.gr  
> - Konstantinos Papakostas  – Konstantinos.Papakostas@etu.sorbonne-universite.fr
>
> **Supervisor:** Antonis Dimitriou  
> Aristotle University of Thessaloniki – May 2023

---

## ⚖️ License

This project is distributed under the [MIT License](./LICENSE).  
Feel free to use, modify, and share for educational purposes.
