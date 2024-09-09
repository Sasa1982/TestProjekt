
//Gameplan:
// 1. Beim laden der CocktailCombinator-Page wird eine Platzhalter Objekt für einen Cocktail erstellt und mit 0en initialisiert
// 2. Input durch Nutzer überschreibt dann wo nötig die nötigen Stellen
// 3. Abschließend wird der Cocktail benannt und erst dann gespeichert (sonst Schwemmen wir den Speicher versehentlich mit leeren Objekten)



//Variablen

//Row1
//const grenadineButton = document.getElementById ("Button-Grenadine");
//const blueButton      = document.getElementById ("Button-Blue");
//const limeButton      = document.getElementById ("Button-Lime");
//const cocosButton     = document.getElementById ("Button-Cocos");

//Row2
//const mandelButton    = document.getElementById ("Button-Mandel");
//const sechsButton     = document.getElementById ("Button-6");
//const siebenButton    = document.getElementById ("Button-7");
//const achtButton      = document.getElementById ("Button-8");

//Row3
//const ananasButton    = document.getElementById ("Button-Ananas");
//const organgeButton   = document.getElementById ("Button-Orange");
//const maracujaButton  = document.getElementById ("Button-Maracuja");
//const cranberryButton = document.getElementById ("Button-Cranberry");

//Row4
//const zitroneButton   = document.getElementById ("Button-Zitrone");
//const gineButton      = document.getElementById ("Button-gin");
//const tripleSecButton = document.getElementById ("Button-Triple-Sec");
//const sechzehnButton  = document.getElementById ("Button-16");

//Row5
//const wodkaButton     = document.getElementById ("Button-Wodka");
//const cachacaButton   = document.getElementById ("Button-Cachaca");
//const rumWeissButton  = document.getElementById ("Button-Rum-weiss");
//const rumBraunButton  = document.getElementById ("Button-Rum-Braun");

//Row6
//const tequilaButton   = document.getElementById ("Button-Tequila");
//const pfirischButton  = document.getElementById ("Button-Pfirsich");
//const dreichungszwanzigButton = document.getElementById ("Button-23");
//const sahneButton     = document.getElementById ("Button-Sahne");

//Row7
//const fertigButton    = document.getElementById ("Button-Fertig");



const cocktail = {          //Objekt vom Typ Cocktail kann nicht geändert werden, die einzelnen Werte im Cocktail aber schon

    "name": "unbekannt",    //Platzhalter, wird überschrieben
    "use": 1,
    "plu": "000",
    "group": 0,
    "color": 3,         // <-- vielleicht andere vorgefertigte Farbe für Eigenerstelltes, nur zur Unterscheidung?
    "comment": " ",
    "zutaten": [
        {
            "name": "Grenadine",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Blue",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Lime",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Cocos",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Mandel",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "6",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "7",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "8",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Ananas",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Orange",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Maracuja",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Cranberry",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Zitrone",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Gin",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Triple",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "16",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Wodka",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Cachaca",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Rum weiss",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Rum braun",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Tequila",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Pfirsich",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "23 ",
            "ml": [
                0,
                0
            ]
        },
        {
            "name": "Sahne",
            "ml": [
                0,
                0
            ]
        }
    ]
}


//Funktion um Zutaten zu kombinieren

//Index gibt an welche Zutat avisiert wird. Muss zu Beginn "leer" initialisiert werden
let currentZutatIndex = null;

document.addEventListener('DOMContentLoaded', () => {
    cocktail.zutaten.forEach((zutat, index) => {
        const zutatName = zutat.name.toLowerCase();                                                     //weil ich die Buttons leider mit Großbuchstaben geschrieben habe
        const buttonID  = 'button-' + ${zutatName};                                                        //versuche Fehler abzufangen
        console.log('Suche Button mit ID: ' + ${buttonId});
        const button = document.getElementById(buttonID);

        if(button) {
            button.addEventListener ('click', () => {
                currentZutatIndex = index;
                document.getElementById('reaktiv').innerText = `${zutat.name} bearbeiten`;
                document.getElementById('inputMengeml').value = '';
                document.getElementById('checkErsterLauf').checked = false;
            });
        }

        else {
            console.log('Button with ID $(buttonID} not found');
        }
    }
)


    document.getElementById('save-button').addEventListener('click', () => {                            //Event Listener für den Save-Button im Modal
        const menge = parse(document.getElementById('inputMengeml').value);                        //holt Menge aus dem Eingabefeld im Modal
        const isErsteMenge = document.getElementById('checkErsterLauf').checked;                        //erste Menge Checkbox auswerten um zu gucken ob es zur ersten oder zweiten Zahl kommt

        if (currentZutatIndex !== null) {
            if (isErsteMenge==true) {
                cocktail.zutaten[currentZutatIndex].ml[0] = menge;
            } else {
                cocktail.zutaten[currentZutatIndex].ml[1] = menge;
            }

            console.log(cocktail)
            document.getElementById('mengeMeldung').innerText = zutatenName + " " + inputMengeml + " hinzugefügt."
//            alert(`Menge für ${cocktail.zutaten[currentZutatIndex].name} aktualisiert!`);               //alert mit welcher Menge was hinzugefügt wurde, hoffentlich responsiv, funktioniert noch nicht
//            $('#editModal').modal('hide');
        } else {
            alert('Bitte eine gültige Zahl eingeben.');                                                 //kann wahrscheinlich raus wenn wir ein reines Int Feld nehmen, aber nur zur Sicherheit damit es im Zweifelsfall nicht crasht
        }
    });
});




//Moin Sascha, hier anbei die Fragmente für den File Upload wo der das postet. Laut Lehrbuch läuft das über "AJAX requests" von beiden Seiten

//Das Erste ist vonseite des Javascripts was den Upload anstoßen soll

document.getElementById('button-fertig').addEventListener('click', () => {
   fetch('/save-cocktail', {                                                                           //muss irgenwie über den Server laufen. Keine Ahnung wie
        method: 'POST',
       headers: {
           'Content-Type': 'application/json'
               },
        body: JSON.stringify(cocktail)
    })
    .then(response => response.text())
    .then(data => alert(data))
    .catch(error => console.error('Error:', error));
})


//Das Zweite hier hab ich schon mal die KI zusammenschießen lassen für nen groben Eindruck. In etwa so sollte der Serverseitige Code zum Entgegennehmen aussehen

const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(express.json());

app.post('/save-cocktail', (req, res) => {
    const cocktail = req.body;
    const filePath = path.join(__dirname, 'cocktails', `${cocktail.name}.json`);

    fs.writeFile(filePath, JSON.stringify(cocktail, null, 2), (err) => {
        if (err) {
            console.error('Error saving cocktail:', err);
            return res.status(500).json({ success: false, message: 'Error saving cocktail.' });
        }
        res.json({ success: true, message: 'Cocktail saved successfully!' });
    });
});

app.listen(3000, () => {
    console.log('Server running on http://localhost:3000');
});
