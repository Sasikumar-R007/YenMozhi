// let model, labelContainer, maxPredictions;

// async function init() {
//   console.log("Loading model...");
//   const URL = "https://teachablemachine.withgoogle.com/models/I2lHu7Veq/";
//   const modelURL = URL + "model.json";
//   const metadataURL = URL + "metadata.json";

//   try {
//     model = await tm.sound.load(modelURL, metadataURL);
//     maxPredictions = model.getTotalClasses();

//     labelContainer = document.getElementById("label-container");
//     labelContainer.innerHTML = "";

//     for (let i = 0; i < maxPredictions; i++) {
//       labelContainer.appendChild(document.createElement("div"));
//     }

//     model.listen((result) => {
//       let highest = result.reduce((max, res) =>
//         res.probability > max.probability ? res : max
//       );

//       labelContainer.childNodes[0].innerHTML =
//         "Prediction: " + highest.className;
//       labelContainer.childNodes[1].innerHTML =
//         "Confidence: " + (highest.probability * 100).toFixed(2) + "%";
//     }, {
//       probabilityThreshold: 0.7,
//       overlapFactor: 0.5
//     });
//   } catch (e) {
//     console.error("Model loading failed:", e);
//   }
// }
