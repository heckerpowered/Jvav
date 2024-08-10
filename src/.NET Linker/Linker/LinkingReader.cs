using System;
using System.Text;

namespace Jvav.Linker;

public class LinkingReader(Stream inputStream)
{
    private readonly BinaryReader reader = new(inputStream);
    public Stream InputStream { get; } = inputStream;

    public ulong ReadLinkerVersion()
    {
        return reader.ReadUInt64();
    }

    public V1.Linking LinkV1()
    {
        return new V1.Linking(InputStream);
    }
}
