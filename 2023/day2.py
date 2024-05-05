import re

id_sum:int = 0
power_sum:int = 0
# predefined bag config
CONST_R:int = 12
CONST_G:int = 13
CONST_B:int = 14

with open("day2_input.txt", "r") as file:
    for game_id, line in enumerate(file, 1):
        game: list = re.split("[:;]", line)[1:]
        flag: bool = True

        r_max:int; b_max:int; g_max:int
        r_max, b_max, g_max = 0,0,0

        for pickset in game:
            r:int; b:int; g:int;
            r, g, b = CONST_R, CONST_G, CONST_B
            
            substr: list = pickset.split(" ")[1:]
            for i in range(0, len(substr) - 1, 2):
                val:int = int(substr[i])
                match substr[i+1][0]:
                    case 'r':
                        r -= val
                    case 'g':
                        g -= val
                    case 'b':
                        b -= val
                        
            r_max = max(r_max, CONST_R - r)
            g_max = max(g_max, CONST_G - g)
            b_max = max(b_max, CONST_B - b)

            if r < 0 or g < 0 or b < 0:
                flag = False

        if flag:
            id_sum += game_id

        power_sum += r_max * g_max * b_max

print(f"""Part 1 - Sum of legal game ID's: {id_sum}
Part 2 - Sum of power of sets: {power_sum}""")