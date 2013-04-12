{
	"main-font" : "segoepr.ttf",
	
	"top-panel": "top_bar.png",
	
	"bottom-panel": "bottom_panel.png",
	
	"buttons" : {
		"upgrade" : {
			"position": [340, 520],
			"image": "button/ButtonUpgrade.png"
		},
		"sell": {
			"position": [440, 520],
			"image": "button/ButtonSell.png"		
		}
	},
	
	"tower-buttons" : [
		{
			"image" : "button/ButtonArcher.png",
			"position" : [40, 520],
			"tower" : 0
		},
		{
			"image" : "button/ButtonCanon.png",
			"position" : [120, 520],
			"tower" : 1
		},
		{
			"image" : "button/ButtonTea.png",
			"position" : [200, 520],
			"tower" : 2
		}
	],
	
	"text" : {
		"level-name": {
			"position" : [400, 50],
			"font-size": 28
		},
		"lives" : {
			"position" : [690, 18],
			"font-size" : 24		
		},
		"countdown" : {
			"position" : [100, 38],
			"font-size" : 24
		},
		"money": {
			"position": [690, 48],
			"font-size": 24
		}
	},
	
	"decorations": [
		{
			"image": "Emblem.png",
			"position": [230, 26]
		},
		{
			"image": "Emblem.png",
			"position": [520, 26]
		},		
		{
			"image": "Heart.png",
			"position": [660, 22]
		},
		{
			"image": "Coin.png",
			"position": [660, 52]
		}
	],
	
	"win": {
		"background": "Win.png"
	},
	
	"loose": {
		"background": "Loose.png"
	},
	
	"main-menu": {
		"background": "menu/MainMenuBackground.png",
		
		"buttons": [
			"menu/button_start.png",
			"menu/button_options.png",
			"menu/button_quit.png"
		]
	},

    "level-picker": {
        "background": "picker/LevelPickerBackground.png",

        "level-buttons": {
            "start": [110, 190],
            "line-offset": [0, 52],
            "text-offset": [65,9],
            "line-width": 350,
            "font-size": 25,
            "color": "black",
            "color-gray": [128,128,128],
            "red": "picker/DiamondButtonRed.png",
            "green": "picker/DiamondButtonGreen.png",
            "gray": "picker/DiamondButtonGray.png"
        },

        "name": {
            "position": [230, 130],
            "font-size": 28,
            "color": [255, 201, 15]
        },

        "back-button": {
            "position": [20, 550],
            "image": "picker/ButtonBack-normal.png"
        },

        "desc": {
            "position": [425, 320],
            "font-size": 20,
            "color": "black",
        },

        "preview": {
            "position": [447, 124],
        },
    },
    
    "text-display": {
    	"background": "text/TextDisplayBackground.png",
    	
    	"text": {
    		"position": [75, 75],
    		"font-size": 22,
    		"color": "black",
    	},
    },
}
