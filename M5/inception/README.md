# Inception Project

This project sets up a small infrastructure with Docker containers running NGINX, WordPress, and MariaDB.

## Project Structure

```
.
├── Makefile
├── secrets/
│   ├── credentials.txt
│   ├── db_password.txt
│   └── db_root_password.txt
└── srcs/
    ├── .env
    ├── docker-compose.yml
    └── requirements/
        ├── mariadb/
        │   ├── Dockerfile
        │   ├── .dockerignore
        │   ├── conf/
        │   │   └── 50-server.cnf
        │   └── tools/
        │       └── init_db.sh
        ├── nginx/
        │   ├── Dockerfile
        │   ├── .dockerignore
        │   ├── conf/
        │   │   └── nginx.conf
        │   └── tools/
        └── wordpress/
            ├── Dockerfile
            ├── .dockerignore
            ├── conf/
            └── tools/
                └── setup_wordpress.sh
```

## Services

- **NGINX**: Web server with TLSv1.2/TLSv1.3 support, listening on port 443
- **WordPress**: CMS with php-fpm (no NGINX)
- **MariaDB**: Database server

## Prerequisites

- Docker
- Docker Compose
- WSL2 (if on Windows)

## Setup Instructions

### 1. Configure Domain Name

Add the following line to your hosts file to point `maia.42.fr` to localhost:

**On WSL/Linux:**
```bash
echo "127.0.0.1 maia.42.fr" | sudo tee -a /etc/hosts
```

**On Windows (as Administrator):**
Add to `C:\Windows\System32\drivers\etc\hosts`:
```
127.0.0.1 maia.42.fr
```

### 2. Update Environment Variables

Edit `srcs/.env` and update the following with your own values:
- Replace `maia` with your login in `DOMAIN_NAME`
- Change all passwords to secure values
- Update email addresses

### 3. Create Data Directories

The volumes will be stored in `/home/maia/data/`. This directory will be created automatically by the Makefile.

### 4. Build and Run

```bash
make
```

Or use individual commands:
```bash
make build    # Build Docker images
make up       # Start containers
make down     # Stop containers
make clean    # Remove containers and volumes
make fclean   # Full cleanup
make re       # Rebuild everything
```

## Access

Once running, access your WordPress site at:
- **HTTPS**: https://maia.42.fr

You'll see a browser warning about the self-signed certificate - this is normal. Accept it to proceed.

## WordPress Credentials

Check `secrets/credentials.txt` for:
- WordPress admin credentials
- WordPress user credentials
- Database credentials

**Default admin user**: `maiawp` (does not contain 'admin' as required)

## Important Notes

1. **Security**: All passwords in `.env` and `secrets/` should be changed before deployment
2. **Domain**: Replace `maia` with your actual login throughout the project
3. **Data Persistence**: WordPress files and database are stored in volumes at `/home/maia/data/`
4. **TLS**: Using self-signed certificates (for production, use Let's Encrypt)
5. **Network**: All containers communicate through the `inception` bridge network
6. **Auto-restart**: Containers are configured to restart automatically on crash

## Troubleshooting

### View logs
```bash
make logs
```

### Check container status
```bash
make ps
```

### Restart services
```bash
make restart
```

### Full reset
```bash
make fclean
make
```

## Project Compliance

This project follows the subject requirements:
- ✅ Each service runs in a dedicated container
- ✅ Built from Debian Bullseye (stable)
- ✅ Custom Dockerfiles for each service
- ✅ NGINX with TLSv1.2/TLSv1.3 only
- ✅ WordPress with php-fpm (no NGINX)
- ✅ MariaDB only (no NGINX)
- ✅ Two volumes (database and WordPress files)
- ✅ Docker network for container communication
- ✅ Auto-restart on crash
- ✅ No infinite loops or hacky patches
- ✅ Proper PID 1 handling
- ✅ Two WordPress users (admin username doesn't contain 'admin')
- ✅ Environment variables for configuration
- ✅ Secrets stored separately
- ✅ NGINX as sole entry point on port 443
- ✅ No use of 'latest' tag
- ✅ No passwords in Dockerfiles

## License

This project is part of the 42 School curriculum.
