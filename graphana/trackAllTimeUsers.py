from common import get_client_ips

import time
import datetime

from sqlalchemy import create_engine, Column, String, DateTime, func
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

print("Collecting Shadowsocks Information")

Base = declarative_base()


class User(Base):
    __tablename__ = 'users'
    ip_address = Column(String, primary_key=True)
    last_seen = Column(DateTime, default=datetime.datetime.utcnow)

class DBManager:
    def __init__(self, db_url):
        self.engine = create_engine(db_url)
        Base.metadata.create_all(self.engine)
        self.Session = sessionmaker(bind=self.engine)

    def init_db(self):
        """Initialize the database."""
        Base.metadata.create_all(self.engine)

    def get_ips(self):
        """Stub function to get list of IPs - replace with your logic."""
        # Example static IPs. You should replace this method with actual logic.
        out = get_client_ips()
        print(out)
        return out

    def update_db(self):
        """Updates the database based on IP addresses."""
        session = self.Session()
        ips = self.get_ips()
        current_time = datetime.datetime.utcnow()

        for ip in ips:
            # Check if the user exists
            user = session.query(User).filter_by(ip_address=ip).first()

            if user:
                # Update last_seen field
                user.last_seen = current_time
            else:
                # Add new user
                new_user = User(ip_address=ip, last_seen=current_time)
                session.add(new_user)

        session.commit()
        session.close()


class Sampler:
    def __init__(self):
        self.db_manager = DBManager('postgresql://auser:1234@localhost/mydb')

    def sample(self):
        self.db_manager.update_db()


def collect_and_store_metrics():
    sampler = Sampler()
    while True:
        sampler.sample()
        time.sleep(1)


print("Starting now")
collect_and_store_metrics()
