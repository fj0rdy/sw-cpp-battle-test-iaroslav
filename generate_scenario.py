"""Generates a command file for the battle simulation"""

import random
from pathlib import Path


W = 100
H = 100
N = 1000
OUTPUT_FILE = "commands_example.txt"
STAT_LOW = 5
STAT_HIGH = 15
TARGET_MARGIN = 10
SEED = None  # Set an integer to reproduce the same scenario.


def generate_commands():
    uint32_max = 2**32 - 1
    if not (1 <= W <= uint32_max and 1 <= H <= uint32_max):
        raise ValueError("W and H must be positive uint32 values.")
    if not (0 <= N <= min(W * H, uint32_max)):
        raise ValueError("N must fit on the map and within the uint32 ID range.")
    if not (0 <= STAT_LOW <= STAT_HIGH <= uint32_max):
        raise ValueError("Expected 0 <= STAT_LOW <= STAT_HIGH <= uint32 max.")
    if TARGET_MARGIN < 0 or max(W, H) - 1 + TARGET_MARGIN > uint32_max:
        raise ValueError("Expanded target coordinates must fit within uint32.")

    rng = random.Random(SEED)
    # Sample cell indices without replacement, then convert them to coordinates.
    cells = rng.sample(range(W * H), N)
    commands = [f"CREATE_MAP {W} {H}"]

    for unit_id, cell in enumerate(cells, start=1):
        x, y = cell % W, cell // W
        unit_type = rng.choice(("SWORDSMAN", "HUNTER"))
        hp = rng.randint(STAT_LOW, STAT_HIGH)
        strength = rng.randint(STAT_LOW, STAT_HIGH)

        if unit_type == "SWORDSMAN":
            commands.append(f"SPAWN_SWORDSMAN {unit_id} {x} {y} {hp} {strength}")
        else:
            agility = rng.randint(STAT_LOW, STAT_HIGH)
            attack_range = rng.randint(STAT_LOW, STAT_HIGH)
            commands.append(
                f"SPAWN_HUNTER {unit_id} {x} {y} {hp} {agility} {strength} {attack_range}"
            )

        # [0, W + margin) and [0, H + margin) include out-of-map targets.
        target_x = rng.randrange(W + TARGET_MARGIN)
        target_y = rng.randrange(H + TARGET_MARGIN)
        commands.append(f"MARCH {unit_id} {target_x} {target_y}")

    return commands


def main():
    commands = generate_commands()
    # Resolve relative output names next to this script, regardless of cwd.
    output_path = Path(__file__).resolve().parent / OUTPUT_FILE
    output_path.write_text("\n".join(commands) + "\n", encoding="utf-8")
    print(f"Written {len(commands)} commands ({N} units) to {output_path}")


if __name__ == "__main__":
    main()
