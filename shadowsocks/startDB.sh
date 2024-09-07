nstall PostgreSQL if not already installed
sudo pacman -Syu --noconfirm postgresql

# Initialize the PostgreSQL database cluster
sudo -u postgres initdb --locale=en_US.UTF-8 -D /var/lib/postgres/data

# Start PostgreSQL service
sudo systemctl start postgresql
sudo systemctl enable postgresql

# Create a PostgreSQL user and database
sudo -u postgres psql -c "CREATE USER user WITH PASSWORD 'password';"
sudo -u postgres psql -c "CREATE DATABASE mydb;"
sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE mydb TO user;"

# Set PostgreSQL to listen on all interfaces
sudo sed -i "s/#listen_addresses = 'localhost'/listen_addresses = '*'/g" /var/lib/postgres/data/postgresql.conf

# Allow password authentication for all users
echo "host    all             all             0.0.0.0/0               md5" | sudo tee -a /var/lib/postgres/data/pg_hba.conf

# Restart PostgreSQL to apply changes
sudo systemctl restart postgresql

echo "PostgreSQL setup complete. The database is ready for use."

