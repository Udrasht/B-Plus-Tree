import random

# Generate random order of numbers from 1 to 100
random_numbers = list(range(1, 101))
random.shuffle(random_numbers)

# Create a text file and write sentences to it
with open('source_analysis.txt', 'w') as file:
    for num in random_numbers:
        sentence = f'INSERT {num}\n'
        file.write(sentence)

    for num in range(1,100):
        sentence = f'RANGE {num} {num+1}\n'
        file.write(sentence)


print("Text file created successfully.")