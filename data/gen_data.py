import random
from faker import Faker
from datetime import datetime, timedelta

fake = Faker()

# Create or overwrite the transactions.txt file
with open("data/transactions.txt", "w") as f:
    # Write header
    f.write("TransactionID,Amount\n")
    
    # Generate 10000 normal transactions
    for i in range(9995):
        tx_id = f"TX{i+1:06d}"
        amount = round(random.uniform(10, 50000), 2)
        f.write(f"{tx_id},{amount}\n")
    
    # Add 5 suspicious transactions
    for i in range(5):
        tx_id = f"TX{i+1:06d}"
        amount = round(random.choice([ 300000, 400000, 500000,600000,700000,800000,900000,1000000]), 2)
        f.write(f"{tx_id},{amount}\n")

