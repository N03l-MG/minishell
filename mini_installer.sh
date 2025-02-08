#!/bin/bash

# Colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

# Create bin directory if it doesn't exist
mkdir -p "$HOME/bin" || {
    echo -e "${RED}Failed to create $HOME/bin directory${RESET}"
    exit 1
}

# Clean and rebuild the project
echo -e "${GREEN}Building and cleaning minishell...${RESET}"
make allclean

# Check if build was successful
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Build failed! minishell executable not found${RESET}"
    exit 1
fi

# Copy minishell to user's bin directory
echo -e "${GREEN}Installing minishell...${RESET}"
cp "./minishell" "$HOME/bin/" || {
    echo -e "${RED}Failed to copy minishell to $HOME/bin${RESET}"
    exit 1
}
chmod +x "$HOME/bin/minishell" || {
    echo -e "${RED}Failed to make minishell executable${RESET}"
    exit 1
}

# Add to PATH if not already there
if [[ ":$PATH:" != *":$HOME/bin:"* ]]; then
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.zshrc"
    echo -e "${GREEN}Added $HOME/bin to PATH in .bashrc and .zshrc${RESET}"
fi

make fclean

echo -e "${GREEN}Installation complete!${RESET}"
echo -e "Run minishell by typing ${GREEN}minishell${RESET} in your terminal!"