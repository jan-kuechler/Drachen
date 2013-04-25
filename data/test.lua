Log(LogLevel.Msg, "Hello World from Lua!")

RegisterForEvent("TestEvent", function(event)
	Log(LogLevel.Msg, "Event handler called for: " .. event)
	
	--dofile("bla.lua")
end)

--dofile("bla.lua")
