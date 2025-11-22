#!/bin/bash

set -e

# Check if the database is already initialized
if [ ! -d "/var/lib/mysql/mysql" ]; then
    echo "Initializing MariaDB database..."
    
    # Initialize the database
    mysql_install_db --user=mysql --datadir=/var/lib/mysql > /dev/null

    # Start MariaDB temporarily
    mysqld --user=mysql --datadir=/var/lib/mysql --skip-networking &
    pid="$!"

    # Wait for MariaDB to start
    for i in {30..0}; do
        if mysqladmin ping &>/dev/null; then
            break
        fi
        echo "Waiting for MariaDB to start..."
        sleep 1
    done

    if [ "$i" = 0 ]; then
        echo "Failed to start MariaDB"
        exit 1
    fi

    echo "MariaDB started successfully"

    # Create database and users
    mysql -u root << EOF
-- Secure the installation
DELETE FROM mysql.user WHERE User='';
DELETE FROM mysql.user WHERE User='root' AND Host NOT IN ('localhost', '127.0.0.1', '::1');
DROP DATABASE IF EXISTS test;
DELETE FROM mysql.db WHERE Db='test' OR Db='test\\_%';

-- Set root password
ALTER USER 'root'@'localhost' IDENTIFIED BY '${MYSQL_ROOT_PASSWORD}';

-- Create database
CREATE DATABASE IF NOT EXISTS ${MYSQL_DATABASE};

-- Create user and grant privileges
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';
GRANT ALL PRIVILEGES ON ${MYSQL_DATABASE}.* TO '${MYSQL_USER}'@'%';

-- Flush privileges
FLUSH PRIVILEGES;
EOF

    echo "Database initialized successfully"

    # Stop the temporary MariaDB instance
    if ! mysqladmin -u root -p"${MYSQL_ROOT_PASSWORD}" shutdown; then
        kill -s TERM "$pid"
        wait "$pid"
    fi
else
    echo "Database already initialized"
fi

# Start MariaDB in the foreground
echo "Starting MariaDB..."
exec mysqld --user=mysql --datadir=/var/lib/mysql
