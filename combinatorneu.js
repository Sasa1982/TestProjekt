//Combinator Skript

const cocktail = {
    "name": "unbekannt",
    "use": 1,
    "plu": "000",
    "group": 0,
    "color": 3,
    "comment": " ",
    "zutaten": [
        { "name": "Grenadine", "ml": [0, 0] },
        { "name": "Blue", "ml": [0, 0] },
        { "name": "Lime", "ml": [0, 0] },
        { "name": "Cocos", "ml": [0, 0] },
        { "name": "Mandel", "ml": [0, 0] },
        { "name": "6", "ml": [0, 0] },
        { "name": "7", "ml": [0, 0] },
        { "name": "8", "ml": [0, 0] },
        { "name": "Ananas", "ml": [0, 0] },
        { "name": "Orange", "ml": [0, 0] },
        { "name": "Maracuja", "ml": [0, 0] },
        { "name": "Cranberry", "ml": [0, 0] },
        { "name": "Zitrone", "ml": [0, 0] },
        { "name": "Gin", "ml": [0, 0] },
        { "name": "Triple", "ml": [0, 0] },
        { "name": "16", "ml": [0, 0] },
        { "name": "Wodka", "ml": [0, 0] },
        { "name": "Cachaca", "ml": [0, 0] },
        { "name": "Rum-weiss", "ml": [0, 0] },
        { "name": "Rum-braun", "ml": [0, 0] },
        { "name": "Tequila", "ml": [0, 0] },
        { "name": "Pfirsich", "ml": [0, 0] },
        { "name": "23", "ml": [0, 0] },
        { "name": "Sahne", "ml": [0, 0] }
    ]
};

// legt für diue Zutaten einen Index an, damit über die Buttons auf die anvisierte Stelle zugegriffen kann
// grobe Idee: Man drückt den Button für die Zutat, das öffnet ein Modal in welches man dann die Menge und den Lauf eingibt, von da wird das ins Rezept gespeichert
// Im Verlauf überall Errors und console.logs damit man Fehler lokalisieren kann


let currentZutatIndex = null;

document.addEventListener('DOMContentLoaded', () => {
    cocktail.zutaten.forEach((zutaten, index) => {
        const zutatName = zutaten.name.toLowerCase();                                                       //weil die Namen mit Großbuchstaben geschrieben sind        
        const buttonElement = document.getElementById(`Button-${zutatName}`);                               //trifft alle Buttons
        if (buttonElement) {
            buttonElement.addEventListener('click', () => {
                currentZutatIndex = index;
                document.getElementById('reaktiv').innerText = `${zutaten.name} bearbeiten`;                //passt dem Namen des Modals an damit man sieht was man angeclickt hat
                document.getElementById('inputMengeml').value = '';
                document.getElementById('checkErsterLauf').checked = false;
                document.getElementById('modalMengeLauf').style.display = 'block';
            });
        } else {
            console.error(`Button with ID button-${zutatName} not found`);          
        }
    });

    document.getElementById('save-button').addEventListener('click', () => {
        const menge = parseFloat(document.getElementById('inputMengeml').value);
        const isErsteMenge = document.getElementById('checkErsterLauf').checked;

        console.log('Save button clicked');
        console.log(`Current Zutat Index: ${currentZutatIndex}`);
        console.log(`Menge: ${menge}`);
        console.log(`Is Erste Menge: ${isErsteMenge}`);

        if (currentZutatIndex !== null && !isNaN(menge)) {
            if (isErsteMenge) {
                cocktail.zutaten[currentZutatIndex].ml[0] = menge;
            } else {
                cocktail.zutaten[currentZutatIndex].ml[1] = menge;
            }

            const zutatName = cocktail.zutaten[currentZutatIndex].name;
            const alertElement = document.getElementById('addAlert');                                       //zeigt die zuletzt zugefügte Zutat mit Menge an, Alert startet geöffnet mit Platzhalter, derzeit keine Ahnung warum
            if (alertElement) {
                alertElement.innerText = `${zutatName} ${menge} ml hinzugefügt.`;
                alertElement.style.display = 'block';
                setTimeout(() => {
                    alertElement.style.display = 'none';
                }, 3000);
            } else {
                console.error(`Element with ID 'addAlert' not found`);
            }
            document.getElementById('modalMengeLauf').style.display = 'none'; // Schließt das Modal wieder

            // Backdrop vom Modal muss seperat geschlossen werden. Ohne den Code hier funktioniert es nicht, warum auch immer
            const modalBackdrop = document.querySelector('.modal-backdrop');
            if (modalBackdrop) {
                modalBackdrop.remove();
            }

            // Log das Rezept nach jeder Addition einer Zutat. Laut Konsole funktioniert das hier jetzt
            console.log('Updated cocktail recipe:', JSON.stringify(cocktail, null, 2));
        } else {
            alert('Bitte eine gültige Zahl eingeben.');                                                     //catch-error wenn es jemand trotzt HTML-Formatierung schafft nicht-Integers einzugeben. Wenn  der hier triggert funktionieren meistens die Buttons nicht
        }
    });
});


//Coming Soon: Export des fertigen Rezeptes als JSON auf den Datenträger durch den Server. Laut Buch kann Javascript sowas nicht

document.getElementById('save-cocktail-button').addEventListener('click', () => {
    const cocktailName = document.getElementById('cocktail-name-input').value;
    cocktail.name = cocktailName;

    // Convert the cocktail object to a JSON string
    const cocktailJSON = JSON.stringify(cocktail, null, 2);

    // Create a Blob from the JSON string
    const blob = new Blob([cocktailJSON], { type: 'application/json' });

    // Create a FormData object to send the file
    const formData = new FormData();
    formData.append('file', blob, `${cocktailName}.json`);

    // Send the formData to the existing document upload endpoint
    fetch('/upload-endpoint', {  // hier müsste der tatsächliche endpoint hin
        method: 'POST',
        body: formData
    })
    .then(response => response.text())
    .then(data => {
        alert('Cocktail erfolgreich gespeichert!');
        console.log('Server response:', data);
    })
    .catch(error => {
        console.error('Fehler beim Speichern des Cocktails:', error);
        alert('Failed to save cocktail.');
    });
});
