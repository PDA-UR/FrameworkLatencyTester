WIDTH = 1920
HEIGHT = 1080
RECT_WIDTH = 300
RECT_HEIGHT = 200
n = 100

clicked = 0
update = 0

-- there are different fullscreen modes, as well as vsync options: https://love2d-community.github.io/love-api/#callbacks
-- they can be set via conf.lua

function love.load(arg)
	n = tonumber(arg[1]) or 1
	local mode_flags = {}
	mode_flags['fullscreen'] = true
	love.window.setMode(WIDTH, HEIGHT, mode_flags)
	canvas = love.graphics.newCanvas(WIDTH, HEIGHT)
	love.graphics.setCanvas(canvas)
	love.graphics.setColor(0, 0, 0)
	love.graphics.rectangle('fill', 0, 0, WIDTH, HEIGHT)
	love.graphics.setCanvas()
end

function clearRects()
	love.graphics.setColor(0, 0, 0)
	love.graphics.rectangle('fill', 0, 0, WIDTH, HEIGHT)
end

function drawRects()
	love.graphics.setColor(0, 0, 0)
	love.graphics.rectangle('fill', 0, 0, WIDTH, HEIGHT)
	for i=1,n-1 do
		local r = math.random(0, 1)
		local g = math.random(0, 1)
		local b = math.random(0, 1)
		local x = math.random(RECT_WIDTH, WIDTH - RECT_WIDTH)
		local y = math.random(0, HEIGHT - RECT_HEIGHT)
		love.graphics.setColor(r, g, b)
		love.graphics.rectangle('fill', x, y, RECT_WIDTH, RECT_HEIGHT)
	end
	love.graphics.setColor(1, 1, 1)
	love.graphics.rectangle('fill', 0, 0, RECT_WIDTH, HEIGHT)
end

function love.mousepressed(x, y, button)
	if clicked == 0 then
		clicked = 1
		update = 1
	end
end

function love.mousereleased(x, y, button)
	if clicked == 1 then
		clicked = 0
		update = 1
	end
end

function love.draw()
	if update == 1 then
		love.graphics.setCanvas(canvas)
		if clicked == 1 then
			drawRects()
		else
			clearRects()
		end
		love.graphics.setCanvas()
		update = 0
	end
	love.graphics.draw(canvas)
end
