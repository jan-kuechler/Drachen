{
    "level-packs": {
        "pack1": {
            "name": "Der Ostwald",
            "desc": "Eine Horde von Banditen\nschickt sich an, die Ost-\ngrenze des Herzogtums zu\nüberfallen.\nWehrt den Angrif aus dem\nOstwald mit Euren Türmen\nab!",
            "image": "pack1/pack.png",
            "levels": [
                ["Kapitel 1" ,"pack1/level1.js"],
                ["Kapitel 2" ,"pack1/level2.js"],
                ["Kapitel 3" ,"pack1/level3.js"],
                ["Kapitel 4", "pack1/level4.js"]
            ],
            "texts": {
            	"pre-pack": "#F:pack1/pre-pack.txt",
            	"pre-level-1": "Bla",
            },
        },
        "eastwood": {
            "name": "Der Ostwald",
            "desc": "Eine Horde von Banditen\nschickt sich an, die Ost-\ngrenze des Herzogtums zu\nüberfallen.\nWehrt den Angrif aus dem\nOstwald mit Euren Türmen\nab!",
            "image": "eastwood/EastwoodPack.png",
            "levels": [
                ["Kapitel 1" ,"eastwood/day1.js"],
                ["Kapitel 2" ,"eastwood/day2.js"],
                ["Kapitel 3" ,"eastwood/day3.js"],
                ["Kapitel 4", "eastwood/night1.js"],
                ["Kapitel 5", "eastwood/night2.js"],
            ],
            "texts": {
            	"pre-pack": "#F:eastwood/pre-pack.txt",
            	"pre-level-4": "#F:eastwood/night-intro.txt",
            },
        },
        "vilage": {
            "name": "Dorf",
            "desc": "Eine Horde von Banditen\nschickt sich an, die Ost-\ngrenze des Herzogtums zu\nüberfallen.\nWehrt den Angrif aus dem\nOstwald mit Euren Türmen\nab!",
            "image": "vilage/VilagePack.png",
            "levels": [
                ["Kapitel 1" ,"eastwood/day1.js"],
                ["Kapitel 2" ,"eastwood/day2.js"],
                ["Kapitel 3" ,"eastwood/day3.js"],
                ["Kapitel 4", "eastwood/night1.js"],
                ["Kapitel 5", "eastwood/night2.js"],
            ],
            "texts": {
            	"pre-pack": "#F:eastwood/pre-pack.txt",
            	"pre-level-4": "#F:eastwood/night-intro.txt",
            },
        },
    },
    "pack-order": [
    	"eastwood", "vilage",
    ],
}
