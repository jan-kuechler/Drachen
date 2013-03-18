{
	"towers" : [
		{
			"name" : "Archer Tower",
			"type" : "archer",
			"base-cost": 100,
			"stages" : [
				{
					"base": "tower/Archer_level1.png",
					"center": [15, 65],
                    "projectile": "arrow.png",
					"range": 100,
					"cooldown": 1.0,
					"attacks": 1,
					"attack-position": [
						[15, 40]
					],
					"power": 2
				},
				{
					"base": "tower/Archer_level2.png",
					"center": [15, 65],
                    "projectile": "arrow.png",
					"range": 120,
					"cooldown": 0.9,
					"attacks": 2,
					"attack-position": [
						[15, 40], [35, 38]
					],					
					"power": 3
				},
				{
					"base": "tower/Archer_level3.png",
					"center": [27, 65],
                    "projectile": "arrow.png",
					"range": 150,
					"cooldown": 0.8,
					"attacks": 3,
					"attack-position": [
						[30, 40], [50, 38], [6, 44]
					],					
					"power": 4
				}
			]
		},
		{
			"name" : "Canon Tower",
			"type" : "canon",
			"base-cost": 100,
			"stages" : [
				{
					"base": "tower/Gun_level1.png",
					"center": [15, 50],
                    "projectile": "ball.png",
					"range": 100,
					"cooldown": 3.0,
					"attacks": 1,
					"attack-position": [
						[25, 3]
					],
					"power": 4,
					"speed": 75,
					"splash-power": 1,
					"splash-range": 25
				},
				{
					"base": "tower/Gun_level2.png",
					"center": [15, 50],
                    "projectile": "ball.png",
					"range": 120,
					"cooldown": 2.0,
					"attacks": 1,
					"attack-position": [
						[10, 15]
					],
					"power": 5,
					"speed": 75,
					"splash-power": 2,
					"splash-range": 35
				}				
			]
		},
		{
			"name" : "Tea Tower",
			"type" : "tea",
			"base-cost": 100,
			"stages" : [
				{
					"base": "tower/Tea_level1.png",
					"center": [15, 55],
                    "projectile": "ball.png",
					"range": 50,
					"cooldown": 3.0,
					"attacks": 1,
					"attack-position": [
						[25, 3]
					],
					"power": 2
				},
				{
					"base": "tower/Tea_level2.png",
					"center": [15, 55],
                    "projectile": "ball.png",
					"range": 55,
					"cooldown": 2.0,
					"attacks": 1,
					"attack-position": [
						[10, 15]
					],
					"power": 4
				},
				{
					"base": "tower/Tea_level3.png",
					"center": [15, 72],
                    "projectile": "ball.png",
					"range": 60,
					"cooldown": 1.0,
					"attacks": 1,
					"attack-position": [
						[10, 15]
					],
					"power": 4
				}					
			]
		}		
	]
}
