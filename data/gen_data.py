import random
from faker import Faker
from datetime import datetime, timedelta

fake = Faker()

# Create or overwrite the transactions.txt file
with open("data/transactions.txt", "w") as f:
    # Write header
    f.write("TransactionID,Amount\n")
    
    # Generate 100000 normal transactions with integer IDs
    for i in range(99995):
        tx_id = i + 1  # Simple integer ID starting from 1
        amount = round(random.uniform(10, 50000), 2)
        f.write(f"{tx_id},{amount}\n")
    
    # Add 5 suspicious transactions with integer IDs
    for i in range(5):
        tx_id = 99996 + i  
        amount = round(random.choice([300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000]), 2)
        f.write(f"{tx_id},{amount}\n")

