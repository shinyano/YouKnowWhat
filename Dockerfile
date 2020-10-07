FROM gcc:10
WORKDIR /app/
COPY ./* ./
RUN gcc cifa.c -o cifa
RUN chmod +x cifa