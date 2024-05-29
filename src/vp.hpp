namespace VP {
    // Init SDL 
    // On error return false 
    [[nodiscard]] bool init() noexcept;
    [[nodiscard]] bool isRunning() noexcept;
    void stopRunning() noexcept;
    void close();
}
