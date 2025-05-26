# First stage: Build the project
FROM debian:bookworm-slim as build

# Set the working directory for the build stage
WORKDIR /src

# Copy project files into the container
COPY CMakeLists.txt /src
COPY simulation /src/simulation
COPY include /src/include
#COPY many.ini /src/many.ini

# Install necessary dependencies for building
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && apt-get clean

# Build the project
WORKDIR /src/build
RUN cmake -DENABLE_COVERAGE=ON .. && make

# Second stage: Minimal runtime environment
FROM debian:bookworm-slim as run

# Set the working directory for the runtime stage
WORKDIR /app

# Copy the built binaries and other necessary files from the build stage
COPY --from=build /src/build/disease_simulation /app
COPY --from=build /src/build/disease_tests /app
#COPY --from=build /src/many.ini /app

# Optional: Run tests to verify everything works correctly (can be skipped in production)
RUN /app/disease_tests

# Set the working directory for runtime execution
WORKDIR /scratch

# Command to execute the simulation on container start
ENTRYPOINT ["/app/disease_simulation"]