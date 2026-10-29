

function start()
    print("Script start")
end

function update(dt)

    if input.is_key_down(Key.W) then
        transform.position.z =
            transform.position.z - 5.0 * dt
    end

    if input.is_key_down(Key.S) then
        transform.position.z =
            transform.position.z + 5.0 * dt
    end

    if input.is_key_pressed(Key.Space) then
        print("He pulsado espacio")
    end

    if input.is_key_released(Key.Space) then
        print("He soltado espacio")
    end

end