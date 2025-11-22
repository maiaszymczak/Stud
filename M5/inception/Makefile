# Inception - Docker Infrastructure Project

.PHONY: all build up down start stop restart clean fclean re logs ps

# Default target
all: build up

# Create data directories
data:
	@echo "Creating data directories..."
	@mkdir -p /home/maia/data/mariadb
	@mkdir -p /home/maia/data/wordpress
	@echo "Data directories created successfully"

# Build all images
build: data
	@echo "Building Docker images..."
	@docker-compose -f srcs/docker-compose.yml build
	@echo "Build completed successfully"

# Build and start containers
up: data
	@echo "Starting containers..."
	@docker-compose -f srcs/docker-compose.yml up -d
	@echo "Containers started successfully"

# Build and start containers (with logs)
up-logs: data
	@echo "Starting containers with logs..."
	@docker-compose -f srcs/docker-compose.yml up --build

# Stop containers
down:
	@echo "Stopping containers..."
	@docker-compose -f srcs/docker-compose.yml down
	@echo "Containers stopped successfully"

# Start existing containers
start:
	@echo "Starting existing containers..."
	@docker-compose -f srcs/docker-compose.yml start
	@echo "Containers started successfully"

# Stop running containers without removing them
stop:
	@echo "Stopping running containers..."
	@docker-compose -f srcs/docker-compose.yml stop
	@echo "Containers stopped successfully"

# Restart containers
restart:
	@echo "Restarting containers..."
	@docker-compose -f srcs/docker-compose.yml restart
	@echo "Containers restarted successfully"

# View logs
logs:
	@docker-compose -f srcs/docker-compose.yml logs -f

# List running containers
ps:
	@docker-compose -f srcs/docker-compose.yml ps

# Clean containers and networks
clean:
	@echo "Cleaning containers and networks..."
	@docker-compose -f srcs/docker-compose.yml down -v
	@echo "Cleanup completed successfully"

# Full cleanup including images and data
fclean: clean
	@echo "Performing full cleanup..."
	@docker system prune -af
	@if [ -d /home/maia/data ]; then \
		echo "Removing data directories..."; \
		sudo rm -rf /home/maia/data/mariadb/* /home/maia/data/wordpress/*; \
		echo "Data directories cleaned"; \
	fi
	@echo "Full cleanup completed"

# Rebuild everything
re: fclean all

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build and start all containers (default)"
	@echo "  build     - Build all Docker images"
	@echo "  up        - Start all containers in background"
	@echo "  up-logs   - Start all containers with logs"
	@echo "  down      - Stop and remove all containers"
	@echo "  start     - Start existing containers"
	@echo "  stop      - Stop running containers"
	@echo "  restart   - Restart all containers"
	@echo "  logs      - View container logs"
	@echo "  ps        - List running containers"
	@echo "  clean     - Remove containers and volumes"
	@echo "  fclean    - Full cleanup including images and data"
	@echo "  re        - Rebuild everything from scratch"
	@echo "  help      - Show this help message"
