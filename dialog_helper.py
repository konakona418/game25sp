import json
import os

def hex_to_rgba_tuple(hex_string):
    """Converts a hex color string (e.g., #RRGGBBAA or #RRGGBB) to a tuple (R, G, B, A)."""
    hex_string = hex_string.lstrip('#')
    if len(hex_string) == 6:
        # Assume opaque if no alpha is provided
        hex_string += 'FF'
    if len(hex_string) != 8:
        raise ValueError("Invalid hex color string. Must be #RRGGBB or #RRGGBBAA.")

    r = int(hex_string[0:2], 16)
    g = int(hex_string[2:4], 16)
    b = int(hex_string[4:6], 16)
    a = int(hex_string[6:8], 16)
    return (r, g, b, a)

def display_speaker(speaker_data):
    """Prints speaker details in a readable format."""
    print(f"  ID: {speaker_data.get('id', 'N/A')}")
    print(f"  Name: {speaker_data.get('name', 'N/A')}")
    print(f"  Color: {speaker_data.get('color', 'N/A')}")
    print(f"  Portrait: {speaker_data.get('portrait', 'N/A')}")
    scaling = speaker_data.get('portrait_scaling_factor', [1.0, 1.0])
    print(f"  Scaling: X={scaling[0]}, Y={scaling[1]}")
    texture_rect = speaker_data.get('texture_rect', 'N/A')
    if texture_rect != 'N/A':
        print(f"  Texture Rect: [{texture_rect[0]}, {texture_rect[1]}, {texture_rect[2]}, {texture_rect[3]}]")


def display_line(line_data):
    """Prints line details in a readable format."""
    print(f"  Speaker ID: {line_data.get('speaker_id', 'N/A')}")
    print(f"  Text: {line_data.get('text', 'N/A')}")

def create_speaker():
    """Interactively creates a new speaker dictionary."""
    print("\n--- Add New Speaker ---")
    speaker = {}
    speaker['id'] = input("Enter Speaker ID (integer): ")
    while not speaker['id'].isdigit():
        print("Invalid ID. Please enter an integer.")
        speaker['id'] = input("Enter Speaker ID (integer): ")
    speaker['id'] = int(speaker['id'])

    speaker['name'] = input("Enter Speaker Name: ")

    while True:
        color_hex = input("Enter Speaker Name Font Color (hex, e.g., #FF0000 or #FF0000FF): ").strip()
        if color_hex.startswith('#') and (len(color_hex) == 7 or len(color_hex) == 9):
            try:
                # Validate hex color by attempting conversion (optional, but good practice)
                hex_to_rgba_tuple(color_hex)
                speaker['color'] = color_hex.upper()
                break
            except ValueError:
                print("Invalid hex color format.")
        else:
            print("Invalid hex color format. Must start with '#' and be 7 or 9 characters long.")

    speaker['portrait'] = input("Enter Portrait Path (e.g., res/hero.png, leave empty if none): ").strip()

    while True:
        try:
            scale_x = float(input("Enter Portrait Scaling Factor X (e.g., 1.0): "))
            scale_y = float(input("Enter Portrait Scaling Factor Y (e.g., 1.0): "))
            speaker['portrait_scaling_factor'] = [scale_x, scale_y]
            break
        except ValueError:
            print("Invalid scaling factor. Please enter a number.")

    # --- New texture_rect input ---
    while True:
        try:
            texture_rect_str = input("Enter Texture Rect (x, y, width, height, e.g., 0,0,128,128): ")
            parts = [int(p.strip()) for p in texture_rect_str.split(',')]
            if len(parts) == 4:
                speaker['texture_rect'] = parts
                break
            else:
                print("Invalid format. Please enter exactly four comma-separated integers.")
        except ValueError:
            print("Invalid input. Please enter integers for x, y, width, and height.")

    print("Speaker created:")
    display_speaker(speaker)
    return speaker

def create_line():
    """Interactively creates a new dialogue line dictionary."""
    print("\n--- Add New Dialogue Line ---")
    line = {}
    speaker_id_str = input("Enter Speaker ID for this line (integer): ")
    while not speaker_id_str.isdigit():
        print("Invalid ID. Please enter an integer.")
        speaker_id_str = input("Enter Speaker ID for this line (integer): ")
    line['speaker_id'] = int(speaker_id_str)

    line['text'] = input("Enter Dialogue Text: ")
    print("Line created:")
    display_line(line)
    return line

def save_dialog_data(data, filename):
    """Saves the dialogue data to a JSON file."""
    try:
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        print(f"\nSuccessfully saved dialogue data to '{filename}'")
    except IOError as e:
        print(f"Error saving file: {e}")

def load_dialog_data(filename):
    """Loads dialogue data from a JSON file."""
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            data = json.load(f)
        print(f"\nSuccessfully loaded dialogue data from '{filename}'")
        return data
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
        return None
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON from '{filename}': {e}")
        return None
    except IOError as e:
        print(f"Error loading file: {e}")
        return None

def main():
    dialog_data = {
        "speakers": [],
        "lines": []
    }
    current_filename = None

    print("--- C++ Dialogue System JSON Editor ---")

    while True:
        print("\n--- Main Menu ---")
        print("1. Create New Dialogue Collection")
        print("2. Load Existing Dialogue Collection")
        print("3. Add Speaker")
        print("4. Add Dialogue Line")
        print("5. View Current Dialogue Data")
        print("6. Save Dialogue Data")
        print("7. Save Dialogue Data As...")
        print("8. Exit")

        choice = input("Enter your choice: ")

        if choice == '1':
            dialog_data = {"speakers": [], "lines": []}
            current_filename = None
            print("New dialogue collection created.")
        elif choice == '2':
            filename = input("Enter the JSON file path to load: ")
            loaded_data = load_dialog_data(filename)
            if loaded_data:
                dialog_data = loaded_data
                current_filename = filename
        elif choice == '3':
            speaker = create_speaker()
            dialog_data["speakers"].append(speaker)
        elif choice == '4':
            line = create_line()
            dialog_data["lines"].append(line)
        elif choice == '5':
            print("\n--- Current Speakers ---")
            if not dialog_data["speakers"]:
                print("No speakers added yet.")
            for i, speaker in enumerate(dialog_data["speakers"]):
                print(f"\nSpeaker {i+1}:")
                display_speaker(speaker)

            print("\n--- Current Dialogue Lines ---")
            if not dialog_data["lines"]:
                print("No lines added yet.")
            for i, line in enumerate(dialog_data["lines"]):
                print(f"\nLine {i+1}:")
                display_line(line)
        elif choice == '6':
            if current_filename:
                save_dialog_data(dialog_data, current_filename)
            else:
                print("No file loaded or specified. Please use 'Save Dialogue Data As...' first.")
        elif choice == '7':
            filename = input("Enter filename to save (e.g., my_dialogue.json): ")
            save_dialog_data(dialog_data, filename)
            current_filename = filename
        elif choice == '8':
            print("Exiting program. Goodbye!")
            break
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()