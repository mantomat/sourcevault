# SourceVault

[![CI](https://github.com/mntmtt/sourcevault/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mntmtt/sourcevault/actions/workflows/ci.yml)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=mantomat_sourcevault&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=mntmtt_sourcevault)

SourceVault is a **personal library management system** designed to help individuals organize, categorize, and
prioritize their media collection. Whether you're managing books, articles, videos, or podcasts, SourceVault provides a
centralized platform to store and access your resources efficiently. With features like customizable topics, priority
levels, and favorite markers, SourceVault empowers users to take control of their learning and entertainment materials.

## Features

### Searching & Filtering

- **Search by string** (case-insensitive, supports partial search)
- **Filter by**: Favorites, Topics, Priority
- **Sort by**: Date Added, Date Released, Alphabetical (Author/Title), Priority

### Import & Export

- **Export Library (JSON)**
- **Export filtered view**
- **Import from JSON**

### Localization

Supports multiple languages, including English and Italian.

### Persistence

SourceVault uses a **JSON-based storage model**, ensuring data is saved between sessions and allowing easy backup and
sharing.