from time import localtime, strftime

from twisted.internet import reactor
from twisted.internet.protocol import Factory, Protocol
from twisted.python import failure


class RadioServer(Factory):
    users: set['Line']

    def __init__(self):
        self.users = set()
        print('------------------------------------')
        print('РадиоСервер тренажера Р-187-П1')
        print('Игнатенко И. Д., Литвиненко М. М.')
        print('© Военный учебный центр ЮФУ 2023 год')
        print('------------------------------------')
        print('\nДля выхода нажмите Ctrl+C')

    def buildProtocol(self, _):
        return Line(self.users)


class Line(Protocol):
    users: set['Line']
    ip: None

    def __init__(self, users: set) -> None:
        super().__init__()
        self.users = users
        self.ip = None

    def _now(self):
        return strftime("%H:%M:%S", localtime())

    def connectionMade(self):
        self.users.add(self)
        self.ip = self.transport.getPeer().host
        print(f'{self._now()}: {self.ip} подключен')
        return super().connectionMade()

    def dataReceived(self, data):
        # print(f'{self._now()}: получено {len(data)}б от {self.ip}')
        for u in self.users:
            if u != self:
                # print(f'отправляю {u.ip}')
                u.transport.write(data)

    def connectionLost(self, reason: failure.Failure = ...):
        print(f'{self._now()}: {self.ip} отключен')
        return super().connectionLost(reason)


def main():
    # pylint: disable=no-member
    reactor.listenTCP(52130, RadioServer())
    reactor.run()


if __name__ == '__main__':
    main()
