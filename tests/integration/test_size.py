import subprocess
from pathlib import Path

def test_total_files_and_size(tmp_path):
    # Create files with known sizes
    f1 = tmp_path / "f1.bin"
    f1.write_bytes(b"A" * 10)
    f2 = tmp_path / "f2.bin"
    f2.write_bytes(b"B" * 20)

    # Determine path to the duana executable (two levels up to project root)
    project_root = Path(__file__).resolve().parents[2]
    duana_exe = project_root / "build" / "duana"

    # Run duana executable
    result = subprocess.run(
        [str(duana_exe), str(tmp_path)],
        capture_output=True, text=True
    )

    assert result.returncode == 0
    lines = result.stdout.splitlines()
    assert any("Total files: 2" in line for line in lines)
    assert any("Total size: 30 bytes" in line for line in lines)