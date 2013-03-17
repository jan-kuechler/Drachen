{
	"towers" : [
		{
			"name" : "Archer Tower",
			"type" : "archer",
			"base-cost": 100,
			"stages" : [
				{
					"base": "Archer_level1.png",
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
					"base": "Archer_level2.png",
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
					"base": "Archer_level3.png",
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
		}
	]
}
