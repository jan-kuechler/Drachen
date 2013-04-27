timers = {}
timers_to_remove = {}

function RegisterTimer(when, func)
	timers[func] = when
end

RegisterForUpdate(function(elapsed)
	first_timer = #timers_to_remove
	
	for func, when in pairs(timers) do
		when = when - elapsed
		if when <= 0 then
			Log(LogLevel.Msg, "Calling timer!")
			func()
			timers_to_remove[#timers_to_remove+1] = func
		end
		timers[func] = when
	end
	
	if #timers_to_remove > first_timer then
		for i=first_timer+1, #timers_to_remove do
			timers[timers_to_remove[i]] = nil
		end
	end
end)

------


Log(LogLevel.Msg, "Hello World from Lua!")

RegisterForEvent("TestEvent", function(event)
	Log(LogLevel.Msg, "Event handler called for: " .. event)
end)

str = CreateString()
str:SetText("Hello World!")
str:SetPosition(50, 50)
str:SetColor(Color.Black)

RegisterTimer(5, function()
	str:SetColor(Color(255, 0, 0))
end)
