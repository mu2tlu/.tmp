#include "../include/PollManager.hpp"
#include <algorithm>

struct FdComparator {
    int fd;
    FdComparator(int fd) : fd(fd) {}
    bool operator()(const pollfd& pfd) const {
        return pfd.fd == fd;
    }
};


void PollManager::addFd(int fd, short events) {
    pollfd pfd = {fd, events, 0};
    _fds.push_back(pfd);
}

void PollManager::removeFd(int fd) {
    _fds.erase(std::remove_if(_fds.begin(), _fds.end(), FdComparator(fd)),
               _fds.end());
}

int PollManager::poll(int timeout) { return ::poll(_fds.data(), _fds.size(),timeout) ;}
const std::vector<pollfd>& PollManager::getFds() const {return _fds;}
